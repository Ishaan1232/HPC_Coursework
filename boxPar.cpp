/**
 * @file box.cpp
 * @brief Implements the Box class methods.
 * 
 * This file provides the implementation of the Box class, including
 * methods for particle management, force calculations, and simulation execution.
 */

#include "box.h"
#include <omp.h>

/**
 * Initializes the box with given dimensions
 * 
 * @param x_length Length of the box in the x-direction.
 * @param y_length Length of the box in the y-direction.
 * @param z_length Length of the box in the z-direction.
 */
Box::Box(const double x_length, const double y_length, const double z_length) 
    : Lx(x_length), Ly(y_length), Lz(z_length), N(0) {

}

/**
 * Adds the given particle to the box.
 * Uses /f$ R^2 = x_{ij}^2 + y_{ij}^2 + z_{ij}^2 /f$ where /f$ a_{ij} = a_i - a_j /f$.
 * If /f$ R^2 < 0.25 /f$, the particle is removed.
 * 
 * @param p The particle to be added to the box.
 * @return 'true' if the particle was added sucessfully, 'false' if not.
 */
bool Box::addParticle(Particle& p) {
    particles.push_back(p);             // append particle to vector

    for (int j = 0; j < N; j++) {       // iterate through every other particle
        Particle& p_j = particles[j];
        double d, R2 = 0.0;
        for (int m = 0; m < 3; m++) {
            d = p.r[m] - p_j.r[m];
            R2 += d*d;                  
        }
        if (R2 < 0.25) {
            particles.pop_back();       // remove particle in last position if R^2 < 0.25
            return false;
        }
    }
    N++;                                // Increment number of particles if added successfully
    return true;
}

/**
 * Uses /f$ \mathbf{F}_i = - \nabla \sum_{\substack{j \neq i}}^{N} \phi_{ij}./f$ to calculate
 * the force on particle i, where \frac{d \phi_{ij}}{dx} = -24 \epsilon x_{ij} \frac{\sigma^6}{r_{ij}^8} \left( \frac{2\sigma^6}{r_{ij}^6} - 1 \right).
 * Using Newton's Law, the force exerted on p_i by p_j is equal but opposite to the force exerted on p_j by p_i.
 * Therefore, the total force is calculated on p_i, but also the force exerted by p_i on other particles.
 * 
 * @param p_i The particle to calculate the total force being exerted on
 * @param i The position of the particle in the vector
 */
void Box::calculateF_i(Particle& p_i, int i) {
    double eps, sig, r_ij2, dphi_dx, sig_rij, inv_rij2, force;
    array<double, 3> diff;
        
//    #pragma omp parallel for schedule(static) 
    for (int j = i + 1; j < N; j++) { // Avoid double calculation
        Particle& p_j = particles[j];
        // Determine epsilon and sigma based on particle types
        if (p_i.type == p_j.type) {         // Find the parameters based on the two particles
            if (p_i.type == 0) {
                eps = 3.0; 
                sig = 1.0;
            } else {
                eps = 60.0;
                sig = 3.0;
            }
        } else {
            eps = 15.0;
            sig = 2.0;
        }
        
        for (int m = 0; m < 3; m++) {
            diff[m] = p_i.r[m] - p_j.r[m];      // Calculate distance between the particles in the x, y and z direction
        }

        r_ij2 = diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2];                  // Square of the separation between the particles
        inv_rij2 = 1.0 / r_ij2;                                                             // Calculate 1/r^2
        sig_rij = (sig * sig * sig * sig * sig * sig) * inv_rij2 * inv_rij2 * inv_rij2;     // Calcualte (sigma/r)^6
        dphi_dx = -24 * eps * sig_rij * (2 * sig_rij - 1) * inv_rij2;                       // Calculate the constant in the force equation

        // Apply forces to both particles (equal and opposite)
        for (int m = 0; m < 3; m++) {
            force = -dphi_dx * diff[m];             // Calculate force exerted on p_i by p_j
            p_i.F[m] += force;                        
            p_j.F[m] -= force;                      // Newton's Law                                          
        }
    }
}

/**
 * Calculate the system's kinetic energy bu summing the KE of individiual particles.
 * 
 * @return The kinetic energy of the system
 */
double Box::systemKE() {
    double E = 0.0;
    #pragma omp parallel for reduction(+:E)
    for (int i = 0; i < N; i++) {
        E += particles[i].particleKE();
    }
    return E;
}

/**
 * The simulation is run for T seconds where the particles are updated every timesetep.
 * The kinetic energy is written to a text file every 0.1 seconds.
 * The particle positions in the x-y plane are written to a text file for the test cases.
 * 
 * @param dt The timestep to update the particles
 * @param T The total time to run the simulation
 * @param temp The temperature to be fixed to, however, if equal to -1 not kept constant
 * @param ic_random 'true' if the initial condition is ic_random, 'false' if not, used for writing to text files.
 * @param ic The initial condition being run, used to name the text files appropriately as ic.txt.
 */
void Box::runSimulation(double dt, double T, double temp, bool ic_random, string ic) {
    string particle_file = "Particle_Data/Data/" + ic + ".txt";       
    string KE_file = "KE_Data/Data/" + ic + ".txt";                     
    ofstream particleData(particle_file, ios::out | ios::trunc);
    ofstream KEData(KE_file, ios::out | ios::trunc);                     // Open files for print only.

    double E, lambda;

    if (temp != -1) {       // Check if temperature shoudl be scaled
        E = systemKE();                                                 
        lambda = sqrt((temp * 1.5 * 0.8314459920816467 * double(N))/E);              // Constant to scale velocities before simulation runs to get initial KE correct
        for (int i = 0; i < N; i++) {
            particles[i].scaleTemp(lambda);
        }
    }

    E = systemKE();

    if (!ic_random) {
        for (int i = 0; i < N; i++)  {
            particleData << " " << 0.0
                        << " " << i + 1 
                        << " " << particles[i].r[0]
                        << " " << particles[i].r[1]
                        << " " << particles[i].r[2]
                        << " " << particles[i].get_v()[0]
                        << " " << particles[i].get_v()[1]
                        << " " << particles[i].get_v()[2] << endl;             // Write initial particles trajectories row by row
        }
    }

    KEData  << " " << 0.0 << " " << E << endl;              // Write initial KE to file

    for (double t = dt; t < T + dt; t += dt) {
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < N; i++) {
            particles[i].updatePosition(dt);                                                // Update particle's position first
        }
        
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < N; i++) {
            calculateF_i(particles[i], i);                                     // Calculate force on particle p_i
        }
        
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < N; i++) {
            particles[i].updateVelocity(dt, Lx, Ly, Lz);                       // Update p_i's velocity
        }
        
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < N; i++) {
            particles[i].F = {0.0, 0.0, 0.0};                               // Reset force to zero 
        } 

        if (temp != -1) {
            E = systemKE();                                                 
            lambda = sqrt((temp * 1.5 * 0.8314459920816467 * double(N))/E);              // Constant to scale velocities before simulation runs to get initial KE correct
            for (int i = 0; i < N; i++) {
                particles[i].scaleTemp(lambda);
            }       
        }

        E = systemKE();

        if (fmod(t, 0.1) < dt) {            
            KEData  << " " << round(t * 10) / 10 << " " << E << endl;               // Write KE to file
        }     

        if (!ic_random) {
            if (fmod(t, 0.1) < dt) {
                for (int i = 0; i < N; i++)  {
                    particleData << " " << round(t * 10) / 10
                                << " " << i + 1 
                                << " " << particles[i].r[0]
                                << " " << particles[i].r[1]
                                << " " << particles[i].r[2]
                                << " " << particles[i].get_v()[0]
                                << " " << particles[i].get_v()[1]
                                << " " << particles[i].get_v()[2] << endl;             // Write initial particles trajectories row by row
                }
            }
        }
    }

    particleData.close();
    KEData.close();                 // Close files once finished with simulation
}
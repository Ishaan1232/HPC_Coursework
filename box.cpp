#include "box.h"

Box::Box(const double x_length, const double y_length, const double z_length) 
    : Lx(x_length), Ly(y_length), Lz(z_length) {
        // Nothing extra to do
}

void Box::addParticle(const Particle& p) {
    particles.push_back(p);
}

void Box::removeLastParticle() {
    particles.pop_back();
}

double Box::findR(int i, int j, double diff[3]) {
    const double* r_j = particles[j].get_r(); 
    const double* r_i = particles[i].get_r();  // Get the pointer to r
    double norm = 0.0;
    for (int m = 0; m < 3; m++) {
        diff[m] = r_j[m];
        diff[m] -= r_i[m];
        norm += diff[m] * diff[m];
    }
    
    return sqrt(norm);
}

void Box::calculateF_i(int i) {
    double eps, sig, r_ij, dphi_dx;
    double diff[3];
    double F_i[3] = {0.0, 0.0, 0.0};
    for (int j = 0; j < int(particles.size()); j++) {
        if (i != j) {
            if (particles[i].get_type() == particles[j].get_type()) {
                if (particles[i].get_type() ==  0) {
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

            r_ij = findR(i, j, diff);
            double sig_rij = (sig*sig*sig*sig*sig*sig)/(r_ij*r_ij*r_ij*r_ij*r_ij*r_ij);  // (sigma/r)^6 (avoids pow)
            dphi_dx = 24 * eps * sig_rij * (2 * sig_rij  - 1) / (r_ij*r_ij);

            for (int m = 0; m < 3; m++) {
                F_i[m] -= dphi_dx * diff[m];
            }
            
        }
    }
    particles[i].set_F(F_i);
}

double Box::systemKE() {
    double E = 0.0;
    for (size_t i = 0; i < particles.size(); i++) {
        E += particles[i].particleKE();
    }
    return E;
}

void Box::runSimulation(double dt, double T, double temp, bool ic_random, string ic) {
    string particle_file = "Particle_Data/Data/" + ic + ".txt";
    string KE_file = "KE_Data/Data/" + ic + ".txt";
    ofstream particleData(particle_file, ios::out | ios::trunc);
    ofstream KEData(KE_file, ios::out | ios::trunc);

    int N = particles.size();

    double E = 0.0;
    if (temp != -1) {
        E = systemKE();
        for (int i = 0; i < N; i++) {
            particles[i].scaleTemp(E, temp);
        }
    }

    for (double t = 0; t < T + dt; t += dt) {
        for (int i = 0; i < N; i++) {
            if (fmod(t, 0.1) < dt) {
                if (!ic_random) {
                    particleData << setw(7) << round(t * 10) / 10
                                << setw(7) << i + 1 
                                << setw(15) << particles[i].get_r()[0]
                                << setw(15) << particles[i].get_r()[1]
                                << setw(15) << particles[i].get_r()[2]
                                << setw(15) << particles[i].get_v()[0]
                                << setw(15) << particles[i].get_v()[1]
                                << setw(15) << particles[i].get_v()[2] << endl;
                }
            }
            particles[i].updatePosition(dt);            // update position first
        }

        for (int i = 0; i < N; i++) {
            calculateF_i(i);                                   // force
        }

        E = systemKE();
        if (fmod(t, 0.1) < dt) {
            KEData  << setw(7) << round(t * 10) / 10 << setw(15) << E << endl;
        }

        for (int i = 0; i < N; i++) {
            particles[i].updateVelocity(dt, Lx, Ly, Lz);
            if (temp != -1) {
                particles[i].scaleTemp(E, temp);
            }    
        }        
    }

    particleData.close();
    KEData.close();
}
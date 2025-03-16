/**
 * @file particle.cpp
 * @brief Implements the Particle class methods.
 * 
 * This file provides the implementation of the Particle class,
 * handling updates to position, velocity, forces, and interactions.
 */

#include "particle.h"
#include <omp.h>

/** 
 * Initializes the particle with given position, velocity, and type.
 * The particle's force is initialised to zero.
 * 
 * @param x_i Initial x-coordinate.
 * @param y_i Initial y-coordinate.
 * @param z_i Initial z-coordinate.
 * @param vx_i Initial velocity in x-direction.
 * @param vy_i Initial velocity in y-direction.
 * @param vz_i Initial velocity in z-direction.
 * @param type_i Particle type (0 or 1), which determines mass.
 */
Particle::Particle(double x_i, double y_i, double z_i, double vx_i, double vy_i, double vz_i, int type_i)
    : v{vx_i, vy_i, vz_i}, type(type_i), r{x_i, y_i, z_i}, F{0.0, 0.0, 0.0} {  
    if (type == 0) {
        mass = 1.0;
    } else {
        mass = 10.0;
    }
}   

/**
 * Creates a copy of another Particle instance
 * 
 * @param p The particle bing copied.
 */
Particle::Particle(const Particle& p) 
    : v(p.v), mass(p.mass), type(p.type), r(p.r), F(p.F) {

    }

/** 
 * If the particle position goes out of bounds, it is reflected back into the domain.
 * The velocity is chnaged as required.
 * 
 * @param pos Pointer to the position coordinate to be corrected.
 * @param vel Pointer to the velocity coordinate to be changed.
 * @param L Box dimension for the given coordinate.
 */
void Particle::applyBC(double& pos, double& vel, double L) {
    if (pos < 0) {
        pos = -pos; 
        vel = abs(vel);
    }
    if (pos > L) {
        pos = 2 * L - pos;
        vel = -abs(vel);
    }
}

/**
 * Uses the equation \f$ r = r + v \cdot dt \f$.
 * 
 * @param dt Time step.
 */
void Particle::updatePosition(double dt) {
    #pragma omp simd
    for (int i = 0; i < 3; i++) {
        r[i] += dt * v[i];
    }
    
}

/**
 * Uses the equation \f$ v = v + (F/m) \cdot dt \f$.
 * Also applies boundary conditions to prevent particles from exiting the simulation domain.
 * 
 * @param dt Time step.
 * @param Lx Box dimension in the x-direction.
 * @param Ly Box dimension in the y-direction.
 * @param Lz Box dimension in the z-direction.
 */
void Particle::updateVelocity(double dt, double Lx, double Ly, double Lz) {
    #pragma omp simd
    for (int i = 0; i < 3; i++) {
        v[i] += dt/mass * F[i];
    }
    
    // Apply the boundary conditions
    applyBC(r[0], v[0], Lx);
    applyBC(r[1], v[1], Ly);
    applyBC(r[2], v[2], Lz);
}


/**
 * Uses \f$ v = \lambda \cdot v \f$
 * 
 * @param lambda Scaling factor for velocity adjustment.
 */
void Particle::scaleTemp(double lambda) {
    #pragma omp simd
    for (int i = 0; i < 3; i++) {
        v[i] *= lambda;
    }
}

/**
 * Uses the formula: \f$ KE = \frac{1}{2} m v^2 \f$.
 * 
 * @return The kinetic energy of the particle.
 */
double Particle::particleKE() {
    double speed2 = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
    return 0.5 * mass * speed2;
}

#include "particle.h"

Particle::Particle(double x_i, double y_i, double z_i, double vx_i, double vy_i, double vz_i, int type_i) 
    : v{vx_i, vy_i, vz_i}, F{0.0, 0.0, 0.0}, type(type_i), r{x_i, y_i, z_i} {
    if (type == 0) {
        mass = 1.0;
    } else {
        mass = 10.0;
    }
}   

void Particle::applyBC(double& pos, double& vel, double L) {
    if (pos < 0) {
        pos = -pos;  // Reflect inside the boundary
        vel = abs(vel);
    }
    if (pos > L) {
        pos = 2 * L - pos;
        vel = -abs(vel);
    }
}

void Particle::updatePosition(double dt) {
    for (int i = 0; i < 3; i++) {
        r[i] += dt * v[i];
    }
    
}


void Particle::updateVelocity(double dt, double Lx, double Ly, double Lz) {
    for (int i = 0; i < 3; i++) {
        v[i] += dt/mass * F[i];
    }
    
    // Apply the boundary conditions
    applyBC(r[0], v[0], Lx);
    applyBC(r[1], v[1], Ly);
    applyBC(r[2], v[2], Lz);
}

void Particle::scaleTemp(double E, double temp) {
    double lambda = sqrt((temp * 1.5 * 0.8314459920816467)/E);
    for (int i = 0; i < 3; i++) {
        v[i] *= lambda;
    }
}

double Particle::particleKE() {
    double speed2 = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
    return 0.5 * mass * speed2;
}

void Particle::set_F(const array<double, 3>& F_new) {
    F = F_new;
}
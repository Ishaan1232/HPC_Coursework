#include "particle.h"

Particle::Particle(double x_i, double y_i, double z_i, double vx_i, double vy_i, double vz_i, int type_i) : type(type_i) {  
    r = new double[3]{x_i, y_i, z_i};
    v = new double[3]{vx_i, vy_i, vz_i};
    F = new double[3]{0.0, 0.0, 0.0};  // Initialize force
    if (type == 0) {
        mass = 1.0;
    } else {
        mass = 10.0;
    }
}   

Particle::Particle(const Particle& p) {
    type = p.type;
    mass = p.mass;
    r = new double[3]{p.r[0], p.r[1], p.r[2]};
    v = new double[3]{p.v[0], p.v[1], p.v[2]};
    F = new double[3]{p.F[0], p.F[1], p.F[2]};
}       

Particle::~Particle() {
    delete[] r;
    delete[] v;
    delete[] F;
    r = v = F = nullptr;
}

void Particle::applyBC(double* pos, double* vel, double L) {
    if (*pos < 0) {
        *pos = -*pos;  // Reflect inside the boundary
        *vel = abs(*vel);
    }
    if (*pos > L) {
        *pos = 2 * L - *pos;
        *vel = -abs(*vel);
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
    applyBC(&r[0], &v[0], Lx);
    applyBC(&r[1], &v[1], Ly);
    applyBC(&r[2], &v[2], Lz);
}

void Particle::scaleTemp(double E, double temp) {
    double lambda = sqrt((temp * 1.5 * 0.8314459920816467)/E);
    for (int m = 0; m < 3; m++) {
        v[m] *= lambda;
    }
}

double Particle::particleKE() {
    double speed2 = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
    return 0.5 * mass * speed2;
}

void Particle::set_F(double F_new[3]) {
    for (int m = 0; m < 3; m++) {
        F[m] = F_new[m];
    }
}
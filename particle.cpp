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

void Particle::updatePosition(double dt, double Lx, double Ly, double Lz, double temp, double E) {
    cblas_daxpy(3, dt, v, 1, r, 1);    // update position r = r + dt*v
    cblas_daxpy(3, dt/mass, F, 1, v, 1);  // update velocity v = v + dt/mass * F
    
    // Apply the boundary conditions
    applyBC(&r[0], &v[0], Lx);
    applyBC(&r[1], &v[1], Ly);
    applyBC(&r[2], &v[2], Lz);

    if (temp != -1.0) {
        cblas_dscal(3, sqrt(temp/(2*E/(3 * 0.8314459920816467))), v, 1);
    }
}

double Particle::particleKE() {
    double speed = cblas_dnrm2(3, v, 1);
    return 0.5 * mass * pow(speed, 2);
}

void Particle::set_F(double F_new[3]) {
    cblas_dcopy(3, F_new, 1, F, 1);
}
#include "particle.h"

Particle::Particle(double x_i, double y_i, double z_i, double vx_i, double vy_i, double vz_i, int type_i)
    : r({x_i, y_i, z_i}), v({vx_i, vy_i, vz_i}), F({0.0, 0.0, 0.0}), type(type_i) {
        if (type == 0) {
            mass = 1.0;
        } else {
            mass = 10.0;
        }
    }

Particle::Particle(const Particle& p) {
    r = p.r; v = p.r; F = p.F;
    type = p.type; mass = p.mass;
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

void Particle::updatePosition(double dt, double Lx, double Ly, double Lz, double temp, double E) {
    cblas_daxpy(3, dt, v.data(), 1, r.data(), 1);    // update position r = r + dt*v
    cblas_daxpy(3, dt/mass, F.data(), 1, v.data(), 1);  // update velocity v = v + dt/mass * F
    
    // Apply the boundary conditions
    applyBC(r[0], v[0], Lx);
    applyBC(r[1], v[1], Ly);
    applyBC(r[2], v[2], Lz);

    if (temp != -1.0) {
        double alpha = sqrt(temp/(2*E/(3 * 0.8314459920816467)));
        cblas_dscal(3, alpha, v.data(), 1);
    }
}

double Particle::particleKE() {
    double speed = cblas_dnrm2(3, v.data(), 1);
    return 0.5 * mass * speed * speed;
}
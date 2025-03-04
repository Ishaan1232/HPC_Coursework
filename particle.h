#ifndef PARTICLE_H
#define PARTICLE_H// header guard to prevent being included in multiple files

#include <iostream>
#include <cmath> 
using namespace std;

class Particle {
    public:
    
        double x, y, z;      // Position
        double vx, vy, vz;   // Velocity
        int type;            // Particle type = 0 or 1
        double mass;         // Mass (1 for type = 0, 10 for type = 1)
        
        // Constructor: create particles with its position, velocity and type(mass)
        Particle(double x_i, double y_i, double z_i, double vx_i, double vy_i, double vz_i, int type_i) 
        : x(x_i), y(y_i), z(z_i), vx(vx_i), vy(vy_i), vz(vz_i), type(type_i) {  
            if (type == 0) {
                mass = 1.0;
            } else {
                mass = 10.0;
            }
        }   

        // Updates the particles position and velocity:
        // r(i+1) = r(i) + dt * v(i)
        // v(i+1) = v(i) = dt * F(i)/mass
        // check boundary conditions after
        void updateParticle(double dt, double Lx, double Ly, double Lz, double Fx, double Fy, double Fz) {
            x += dt * vx;    // update x
            y += dt * vy;    // update y
            z += dt * vz;    // update z

            vx += dt * Fx/mass;  // update vx
            vy += dt * Fy/mass;  // update vy
            vz += dt * Fz/mass;  // update vz
            
            //check x boundary condition
            if (x < 0) {x = -x; vx = abs(vx);} 
            if (x > Lx) {x = 2*Lx - x; vx = -abs(vx);}

            //check y boundary condition
            if (y < 0) {y = -y; vy = abs(vy);} 
            if (y > Ly) {y = 2*Ly - y; vy = -abs(vy);}

            //check z boundary condition
            if (z < 0) {z = -z; vz = abs(vz);} 
            if (z > Lz) {z = 2*Lz - z; vz = -abs(vz);}
        }
        
        // calcaultes kinetic energy of particle
        double particleKE() {
            return 0.5 * mass * (vx*vx + vy*vy + vz*vz);
        }
};

#endif // particle.h
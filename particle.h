#ifndef PARTICLE_H
#define PARTICLE_H// header guard to prevent being included in multiple files

#include <iostream>
#include <cmath> 
#include <tuple>
using namespace std;

#include <cblas.h>

class Particle {
    private:
        double* r;      // Position
        double* v;      // Velocity
        double* F;      // Force at time t
        int type;            // Particle type = 0 or 1
        double mass;         // Mass (1 for type = 0, 10 for type = 1)
        void applyBC(double* pos, double* vel, double L);

    public:
        
        // Constructor: create particles with its position, velocity and type(mass)
        Particle(double x_i, double y_i, double z_i, double vx_i, double vy_i, double vz_i, int type_i);

        // Copy Constructor
        Particle(const Particle& p);    
        
        // Destructor
        ~Particle();

        // Updates the particles position and velocity:
        void updatePosition(double dt);
        void updateVelocity(double dt, double Lx, double Ly, double Lz, double temp, double E);
        
        // calcaultes kinetic energy of particle
        double particleKE();

        // getters
        double* get_r() {return r;}
        double* get_v() {return v;}
        double* get_F() {return F;}
        int get_type() {return type;}
        double get_mass() {return mass;}

        // setter
        void set_F(double F_new[3]);
};

#endif // particle.h
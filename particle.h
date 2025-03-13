#ifndef PARTICLE_H
#define PARTICLE_H// header guard to prevent being included in multiple files

#include <iostream>
#include <cmath> 
#include <tuple>
#include <fstream> 
#include <iomanip>
#include <array>
using namespace std;

class Particle {
    private:
        array<double, 3> v;      // Velocity
        array<double, 3> F;      // Force at time t
        int type;            // Particle type = 0 or 1
        double mass;         // Mass (1 for type = 0, 10 for type = 1)
        void applyBC(double& pos, double& vel, double L);

    public:
        array<double, 3> r;      // Position
        // Constructor: create particles with its position, velocity and type(mass)
        Particle(double x_i, double y_i, double z_i, double vx_i, double vy_i, double vz_i, int type_i);

        // Copy Constructor
        Particle(const Particle& p) = default;    
        
        // Destructor
        ~Particle() = default;

        // Updates the particles position and velocity:
        void updatePosition(double dt);
        void updateVelocity(double dt, double Lx, double Ly, double Lz);
        
        // calcaultes kinetic energy of particle
        double particleKE();
        void scaleTemp(double E, double temp);

        // getters
        const array<double, 3>& get_v() {return v;}
        const array<double, 3>& get_F() {return F;}
        const int get_type() {return type;}
        const double get_mass() {return mass;}

        // setter
        void set_F(const array<double, 3>& F_new);
};

#endif // particle.h
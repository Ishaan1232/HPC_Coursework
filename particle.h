#ifndef PARTICLE_H
#define PARTICLE_H// header guard to prevent being included in multiple files

#include <vector>
#include <cmath>
#include <cblas.h>
using namespace std;

class Particle {
    private:
        vector<double> r;       // Position
        vector<double> v;       // Velocity
        vector<double> F;       // Force at time t
        int type;               // Particle type, either 0 or 1
        double mass;            // Mass (1 for type 0 or 10 for type 1)

    public: 
        Particle(double x_i, double y_i, double z_i, double vx_i, double vy_i, double vz_i, int type_i);    // Constructor
        Particle(const Particle& p);    // Copy Constructor
        ~Particle() = default;          // Destructor

        void applyBC(double& pos, double& vel, double L);
        void updatePosition(double dt, double Lx, double Ly, double Lz, double temp, double E);
        double particleKE(); 
        
};

#endif; // PARTICLE_H
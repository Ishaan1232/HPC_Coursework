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
};

#endif; // PARTICLE_H
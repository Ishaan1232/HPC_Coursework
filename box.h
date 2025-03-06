#ifndef BOX_H
#define BOX_H// header guard to prevent being included in multiple files

#include <iostream>
#include <cmath> 
#include <vector>
#include <fstream> 
#include <iomanip>
#include <string>
#include "particle.h"
#include <cblas.h>
using namespace std;

class Box {
    private:
        double Lx, Ly, Lz;  // Box dimensions
        vector<Particle> particles;
        void calculateF_i(int i);

    public: 
        Box(double x, double y, double z); // Constructor
        ~Box() = default;   // Destructor

        void addParticle(const Particle& p);
        void removeLastParticle() ;
        double findR(int i, int j, double diff[3]);
        void runSimulation(double dt, double T, double temp, bool ic_random, const string& ic);
};

#endif // BOX_H
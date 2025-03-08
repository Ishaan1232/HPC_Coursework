#ifndef BOX_H 
#define BOX_H// header guard to prevent being included in multiple files

#include <iostream>
#include <cmath> 
#include <vector>
#include <fstream> 
#include <iomanip>
#include <string>
using namespace std;

#include "particle.h"
#include <cblas.h>

class Box {
    private:
        const double Lx, Ly, Lz;   // box dimensions
        vector<Particle> particles;  // particles stored in a vector
        void calculateF_i(int i);  // Function to calculate F_i

    public:

        Box(const double x_length, const double y_length, const double z_length);  // Constructor to intitialise box
        ~Box() = default;  // Default destructor
        
        void addParticle(const Particle& p);  // Function to add particles to the box
        void removeLastParticle();  // Function to remove the last added particle from the box
        double findR(int i, int j, double diff[3]);  // Function to find the disatnce between two particles i and j

        void runSimulation(double dt, double T, double temp, bool ic_random, string ic);  // Function to run the simulation
};

#endif // BOX_H
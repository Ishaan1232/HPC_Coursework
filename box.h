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

class Box {
    private:
        const double Lx, Ly, Lz;   // box dimensions
        vector<Particle> particles;  // particles stored in a vector
        int N = 0;
        void calculateF_i(int i, bool ic_random);  // Function to calculate F_i

    public:

        Box(const double x_length, const double y_length, const double z_length);  // Constructor to intitialise box
        ~Box() = default;  // Default destructor
        
        bool addParticle(Particle& p);  // Function to add particles to the box
        double systemKE();

        void runSimulation(double dt, double T, double temp, bool ic_random, string ic);  // Function to run the simulation

        // Getters
        double get_Lx() {return Lx;}
        double get_Ly() {return Ly;}
        double get_Lz() {return Lz;}
};

#endif // BOX_H
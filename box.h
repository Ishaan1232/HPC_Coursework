#ifndef BOX_H
#define BOX_H// header guard to prevent being included in multiple files

#include <iostream>
#include <cmath> 
#include <vector>
#include <fstream> 
#include <iomanip>
#include <string>
#include "particle.h"

class Box {
    private:
        const double Lx, Ly, Lz;   // box dimensions
        vector<Particle> particles;  // particles stored in a vector
        int N = 0;

    public:
        Box(const double x_length, const double y_length, const double z_length);  // Constructor to intitialise box
        ~Box() = default;  // Default destructor
        
        bool addParticle(Particle& p);  // Function to add particles to the box
        double findR(int i, int j, double diff[3]);  // Function to find the disatnce between two particles i and j
        double systemKE();

        void runSimulation(double dt, double T, double temp, bool ic_random, string ic);  // Function to run the simulation

        // Getters
        double get_Lx() {return Lx;}
        double get_Ly() {return Ly;}
        double get_Lz() {return Lz;}
        int get_N() {return particles.size();}
};

#endif // BOX_H
#ifndef BOX_H 
#define BOX_H// header guard to prevent being included in multiple files

#include <iostream>
#include <cmath> 
#include <vector>
using namespace std;

#include "particle.h"

class Box {
    public:

        double Lx, Ly, Lz;   // box dimensions
        vector<Particle> particles;  // particles stored in a vector

        // Constructor to intitialise box
        Box(double x_length, double y_length, double z_length) 
            : Lx(x_length), Ly(y_length), Lz(z_length) {
                // nothing to do
        }

        // Destructor
        ~Box() {

        }
        
        // Function to add particles to the box
        void addParticle(const Particle& p) {
            particles.push_back(p);
        }

        void removeLastParticle() {
            particles.pop_back();
        }

        // Function to find the disatnce between two particles i and j
        double findR(int i, int j) {
            double dx = particles[i].x - particles[j].x;  // x separation
            double dy = particles[i].y - particles[j].y;  // y separation
            double dz = particles[i].z - particles[j].z;  // z separation

            return sqrt(dx*dx + dy*dy + dz*dz);
        }

        // Function to find the system's temperature
        double systemTemp(double E) {
            return 2/(3 * 0.8314459920816467) * E;
        }

        // Function to print all particles in the box
        void printParticles() {
            for (size_t i = 0; i < particles.size(); i++) {  // use size_t so i is unsigned like size()
                cout << "Particle " << i + 1 << " Position: ("
                << particles[i].x << ", " << particles[i].y << ", " << particles[i].z << ")" 
                " Type:" << particles[i].type << endl;
            }
        }

        void runSimulation(double T, double dt, double Lx, double Ly, double Lz) {
            for (double t = 0.0; t <= T; t += dt) {
                for (size_t i = 0; i < particles.size(); i++) {
                    
                    particles[i].updatePosition(dt, Lx, Ly, Lz, 0.0, 0.0, 0.0);
                }
            }
        }
};

#endif // BOX_H
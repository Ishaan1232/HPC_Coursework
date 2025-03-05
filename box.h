#ifndef BOX_H 
#define BOX_H// header guard to prevent being included in multiple files

#include <iostream>
#include <cmath> 
#include <vector>
using namespace std;

#include "particle.h"
#include <cblas.h>

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
        double findR(int i, int j, double diff[3]) {
            cblas_dcopy(3, particles[i].r, 1, diff, 1);
            cblas_daxpy(3, -1.0, particles[j].r, 1, diff, 1);
            return cblas_dnrm2(3, diff, 1);
        }

        // Function to calculate F_i

        void calculateF_i(int i, int N) {
            double eps, sig, r_ij, dphi_dx;
            double diff[3];
            cblas_dscal(3, 0.0, particles[i].F, 1);
            for (int j = 0; j < N; j++) {
                if (i != j) {
                    if (particles[i].type == particles[j].type) {
                        if (particles[i].type ==  0) {
                            eps = 3.0; 
                            sig = 1.0;
                        } else {
                            eps = 60.0;
                            sig = 3.0;
                        }
                    } else {
                        eps = 15.0;
                        sig = 2.0;
                    }

                    r_ij = findR(i, j, diff);
                    dphi_dx = -24 * eps * (2*pow(sig/r_ij, 12) - pow(sig/r_ij, 6)) / (r_ij*r_ij);

                    cblas_daxpy(3, -dphi_dx, diff, 1, particles[i].F, 1);
                }
            }
        }

        // Function to print all particles in the box
        void printParticles() {
            for (size_t i = 0; i < particles.size(); i++) {  // use size_t so i is unsigned like size()
                cout << "Particle " << i + 1 << " Position: ("
                << particles[i].r[0] << ", " << particles[i].r[1] << ", " << particles[i].r[2] << ") Type:" << particles[i].type << endl;
            }
        }

        void runSimulation(double Lx, double Ly, double Lz, double dt, double T, int N, double temp) {
            for (double t = 0.0; t <= T; t += dt) {
                for (size_t i = 0; i < particles.size(); i++) {
                    calculateF_i(i, N);
                }
                for (size_t i = 0; i < particles.size(); i++) {
                    particles[i].updatePosition(dt, Lx, Ly, Lz, temp);
                }
            }
        }
};

#endif // BOX_H
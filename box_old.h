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
    public:

        double Lx, Ly, Lz;   // box dimensions
        int N;
        vector<Particle> particles;  // particles stored in a vector

        // Constructor to intitialise box
        Box(double x_length, double y_length, double z_length) 
            : Lx(x_length), Ly(y_length), Lz(z_length) {
                N = 0;
        }

        // Destructor
        ~Box() {

        }
        
        // Function to add particles to the box
        void addParticle(const Particle& p) {
            particles.push_back(p);
            N++;
        }

        void removeLastParticle() {
            particles.pop_back();
            N--;
        }

        // Function to find the disatnce between two particles i and j
        double findR(int i, int j, double diff[3]) {
            cblas_dcopy(3, particles[i].r, 1, diff, 1);
            cblas_daxpy(3, -1.0, particles[j].r, 1, diff, 1);
            return cblas_dnrm2(3, diff, 1);
        }

        // Function to calculate F_i

        void calculateF_i(int i) {
            double eps, sig, r_ij, dphi_dx;
            double diff[3];
            cblas_dscal(3, 0, particles[i].F, 1);
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

        void runSimulation(double Lx, double Ly, double Lz, double dt, double T, double temp, bool ic_random, string ic) {
            string particle_file = "Particle_Data/Data/" + ic + ".txt";
            string KE_file = "KE_Data/Data/" + ic + ".txt";
            ofstream particleData(particle_file, ios::out | ios::trunc);
            ofstream KEData(KE_file, ios::out | ios::trunc);

            for (double t = 0; t < T + dt; t += dt) {
                double E = 0.0;
                for (int i = 0; i < N; i++) {
                    calculateF_i(i);                                   // force at time t, starting at t = 0.0

                    E += particles[i].particleKE();

                    if (fmod(t, 0.1) < dt) {
                        if (!ic_random) {
                            particleData << setw(7) << round(t * 10) / 10
                                        << setw(7) << i + 1 
                                        << setw(15) << particles[i].r[0]
                                        << setw(15) << particles[i].r[1]
                                        << setw(15) << particles[i].r[2]
                                        << setw(15) << particles[i].v[0]
                                        << setw(15) << particles[i].v[1]
                                        << setw(15) << particles[i].v[2] << endl;
                        }
                    }
                }

                if (fmod(t, 0.1) < dt) {
                    KEData  << setw(7) << round(t * 10) / 10 
                            << setw(15) << E << endl;
                }

                for (int i = 0; i < N; i++) {
                    particles[i].updatePosition(dt, Lx, Ly, Lz, temp, E);    // particles at time t + dt
                }

            }


            particleData.close();
            KEData.close();
        }
};

#endif // BOX_H
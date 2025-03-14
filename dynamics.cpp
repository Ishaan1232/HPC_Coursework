/**
 * @file dynamics.cpp
 * @brief This program initializes the simulation by parsing command-line arguments,
 * setting up initial conditions, and running the simulation.
 */

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <cstdlib>
#include "parse_cl.h"
#include "box.h"
#include "particle.h"

/**
 * @brief Generates a random floating-point value within a given range.
 * 
 * @param lower The lower bound of the range.
 * @param upper The upper bound of the range.
 * @return A random double value between `lower` and `upper`.
 */
double rand_val(const double lower, const double upper) {
    return (double(rand()) / RAND_MAX) * (upper - lower) + lower;
}

/**
 * @brief Function to initiliase initial conditions and then run the simulation.
 * 
 * Parses command-line arguemnts to retrieve the parameters, uses the Particle 
 * class to create instances of the particles, then runs the simulation via the Box class
 * 
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return `0` on successful execution, `1` on failure.
 */
int main(int argc, char* argv[]) {
    SimulationParameters para = parseCommandLine(argc, argv);   // Parse command-line arguments

    Box box(para.Lx, para.Ly, para.Lz);                         // Initialise the box

    if (para.ic == "ic-one") {                                  // Initial condition: test case 1
        Particle p(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
        box.addParticle(p);
    } else if (para.ic == "ic-one-vel") {                       // Initial condition: test case 2
        Particle p(10.0, 10.0, 10.0, 5.0, 2.0, 1.0, 0);
        box.addParticle(p);
    } else if (para.ic == "ic-two") {                           // Initial condition: test case 3
        Particle p1(8.5, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
        Particle p2(11.5, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
        box.addParticle(p1);
        box.addParticle(p2);
    } else if (para.ic == "ic-two-pass1") {                     // Initial condition: test case 4
        Particle p1(8.5, 11.5, 10.0, 0.5, 0.0, 0.0, 0);
        Particle p2(11.5, 8.5, 10.0, -0.5, 0.0, 0.0, 0);
        box.addParticle(p1);
        box.addParticle(p2);
    } else if (para.ic == "ic-two-pass2") {                     // Initial condition: test case 5
        Particle p1(8.5, 11.3, 10.0, 0.5, 0.0, 0.0, 0);
        Particle p2(11.5, 8.7, 10.0, -0.5, 0.0, 0.0, 0);
        box.addParticle(p1);
        box.addParticle(p2);
    } else if (para.ic == "ic-two-pass3") {                     // Initial condition: test case 6
        Particle p1(8.5, 11.3, 10.0, 0.5, 0.0, 0.0, 1);
        Particle p2(11.5, 8.7, 10.0, -0.5, 0.0, 0.0, 1);
        box.addParticle(p1);
        box.addParticle(p2);
    } else if (para.ic == "ic-random") {                        // Initial condition: random particles
        int type = 1;
        for (int i = 0; i < para.N; i++) {
            if ((i + 1.0) / para.N > para.percent_type1 / 100) {
                type = 0;                                       // Change particle type to 0
            }
            Particle p(rand_val(0, para.Lx), rand_val(0, para.Ly), rand_val(0, para.Lz),
                       rand_val(-0.5, 0.5), rand_val(-0.5, 0.5), rand_val(-0.5, 0.5), type);   
            if (!box.addParticle(p)) {
                i--;                                            // undo current iteration if particle not added succesfully
            }
        }
        para.ic_random = true;
    }

    box.runSimulation(para.dt, para.T, para.temp, para.ic_random, para.ic);
}

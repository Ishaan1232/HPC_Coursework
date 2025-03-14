#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <cstdlib>
#include "parse_cl.h"
#include "box.h"
#include "particle.h"

double rand_val(const double lower, const double upper) {
    return (double(rand()) / RAND_MAX) * (upper - lower) + lower;
}

int main(int argc, char* argv[]) {
    // Parse command-line arguments
    SimulationParameters para = parseCommandLine(argc, argv);

    Box box(para.Lx, para.Ly, para.Lz);

    if (para.ic == "ic-one") {
        Particle p(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
        box.addParticle(p);
    } else if (para.ic == "ic-one-vel") {
        Particle p(10.0, 10.0, 10.0, 5.0, 2.0, 1.0, 0);
        box.addParticle(p);
    } else if (para.ic == "ic-two") {
        Particle p1(8.5, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
        Particle p2(11.5, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
        box.addParticle(p1);
        box.addParticle(p2);
    } else if (para.ic == "ic-two-pass1") {
        Particle p1(8.5, 11.5, 10.0, 0.5, 0.0, 0.0, 0);
        Particle p2(11.5, 8.5, 10.0, -0.5, 0.0, 0.0, 0);
        box.addParticle(p1);
        box.addParticle(p2);
    } else if (para.ic == "ic-two-pass2") {
        Particle p1(8.5, 11.3, 10.0, 0.5, 0.0, 0.0, 0);
        Particle p2(11.5, 8.7, 10.0, -0.5, 0.0, 0.0, 0);
        box.addParticle(p1);
        box.addParticle(p2);
    } else if (para.ic == "ic-two-pass3") {
        Particle p1(8.5, 11.3, 10.0, 0.5, 0.0, 0.0, 1);
        Particle p2(11.5, 8.7, 10.0, -0.5, 0.0, 0.0, 1);
        box.addParticle(p1);
        box.addParticle(p2);
    } else if (para.ic == "ic-random") {
        for (int i = 0; i < para.N; i++) {
            if ((i + 1.0) / para.N > para.percent_type1 / 100) {
                para.type = 0;
            }
            Particle p(rand_val(0, para.Lx), rand_val(0, para.Ly), rand_val(0, para.Lz),
                       rand_val(-0.5, 0.5), rand_val(-0.5, 0.5), rand_val(-0.5, 0.5), para.type);
            if (!box.addParticle(p)) {
                i--;
            }
        }
        para.ic_random = true;
    }

    box.runSimulation(para.dt, para.T, para.temp, para.ic_random, para.ic);
}

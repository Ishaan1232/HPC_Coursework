#include <iostream>
#include <cmath>
using namespace std;

#include "particle.h"

int main() {
    const double T = 1.0;  // Simulation time
    const double dt = 0.001;  // time step
    const double Lx = 20.0;  // x dimension of box
    const double Ly = 20.0;  // y dimension of box
    const double Lz = 20.0;  // z dimension of box

    Particle p(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
    cout << p.x << endl<< p.y << endl << p.z << endl;
}
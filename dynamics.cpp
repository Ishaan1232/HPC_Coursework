#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
using namespace std;

#include "box.h"
#include "particle.h"

void Help() {
    ifstream file("help.txt");  // Open the fileS

    string line;
    while (getline(file, line)) {  // Read line by line
        cout << line << endl;  // Print each line to terminal
    }

    file.close();  // Close file after reading
}

int main(int argc, char* argv[]) {

    double dt = 0.001;  // default time step
    double Lx = 20.0, Ly = 20.0, Lz = 20.0;  // default dimensions of box
    double percent_type1 = 10.0;  // default percentage of type 1
    double T;    // Final time
    double temp = -1.0; // if temp = -1, the temperature is not fixed
    int N;  // Number of particles

    for (int i = 1; i < argc; i++) {
        
        if (string(argv[i]) == "--help") {
            Help();
            return 0;
        }
    }

    Box box(Lx, Ly, Lz);

    Particle p(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
    box.addParticle(p);

}

// Initial Condition 3
// Particle p1(8.5, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
// Particle p2(11.5, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
    
// box.addParticle(p1);
// box.addParticle(p2);

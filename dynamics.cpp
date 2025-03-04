#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <cstdlib>
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

double check_stod(string value) {
    try 
    {
        double val = stod(value);
        if (val <= 0){
            throw runtime_error("Error: Ensure all values provide are > 0.");
        }
        return val;
    }
    catch(const invalid_argument& e)
    {
        cerr << "Error: Invalid argument - ensure --Lx, --Ly, --Lz, --dt, --T and --percent-type1" <<
        " are given appropriate values of the double type, type --help for more details" << endl;
        exit(1);
    }
}

double check_stoi(string value) {
    try 
    {
        double val = stoi(value);
        if (val <= 0){
            throw runtime_error("Error: Ensure all values provide are > 0.");
        }
        return val;
    }
    catch(const invalid_argument& e)
    {
        cerr << "Error: Invalid argument - ensure --N is given an appropriate integer value, type --help for more details" << endl;
        exit(1);
    }
}

int main(int argc, char* argv[]) {

    double dt = 0.001;  // default time step
    double Lx = 20.0, Ly = 20.0, Lz = 20.0;  // default dimensions of box
    double percent_type1 = 10.0;  // default percentage of type 1
    double T = 0.0;    // Final time
    double temp = -1.0; // if temp = -1, the temperature is not fixed
    int N;  // Number of particles
    int type = 1;
    bool Lx_tick = false, Ly_tick = false, Lz_tick = false, 
                   dt_tick = false, T_tick = false, 
                   ic_tick = false, ic_random_tick = false,
                   percent_type1_tick = false, N_tick = false;   // checks for if mandatory command-line arguements have bee entered

    Box box(Lx, Ly, Lz); // Initialise the simulation box of the Box class

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--help") {
            Help();
            return 0;
        } else if (arg == "--Lx" && i + 1 < argc) {
            Lx = check_stod(argv[++i]);
            Lx_tick = true;
        } else if (arg == "--Ly" && i + 1 < argc) {
            Ly = check_stod(argv[++i]);
            Ly_tick = true;
        } else if (arg == "--Lz" && i + 1 < argc) {
            Lz = check_stod(argv[++i]);
            Lz_tick = true;
        } else if (arg == "--dt" && i + 1 < argc) {
            dt = check_stod(argv[++i]);
            dt_tick = true;
        } else if (arg == "--T" && i + 1 < argc) {
            T = check_stod(argv[++i]);
            T_tick = true;
        } else if (arg == "--ic-one") {
            Particle p(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
            box.addParticle(p);
            N = 1;
            ic_tick = true;
        } else if (arg == "--ic-one-vel") {
            Particle p(10.0, 10.0, 10.0, 0.5, 2.0, 1.0, 0);
            box.addParticle(p);
            N = 1;
            ic_tick = true;
        } else if (arg == "--ic-two") {
            Particle p1(8.5, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
            Particle p2(11.5, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
            box.addParticle(p1);
            box.addParticle(p2);
            N = 2;
            ic_tick = true;
        } else if (arg == "--ic-two-pass1") {
            Particle p1(8.5, 11.5, 10.0, 0.5, 0.0, 0.0, 0);
            Particle p2(11.5, 8.5, 10.0, -0.5, 0.0, 0.0, 0);
            box.addParticle(p1);
            box.addParticle(p2);
            N = 2;
            ic_tick = true;
        } else if (arg == "--ic-two-pass2") {
            Particle p1(8.5, 11.3, 10.0, 0.5, 0.0, 0.0, 0);
            Particle p2(11.5, 8.7, 10.0, -0.5, 0.0, 0.0, 0);
            box.addParticle(p1);
            box.addParticle(p2);
            N = 2;
            ic_tick = true;
        } else if (arg == "--ic-two-pass3") {
            Particle p1(8.5, 11.3, 10.0, 0.5, 0.0, 0.0, 1);
            Particle p2(11.5, 8.7, 10.0, -0.5, 0.0, 0.0, 1);
            box.addParticle(p1);
            box.addParticle(p2);
            N = 2;
            ic_tick = true;
        } else if (arg == "--ic-random") {
            ic_tick = true;
            ic_random_tick = true;
        } else if (arg == "--percent-type1" && i + 1 < argc) {
            percent_type1 = check_stod(argv[++i]);
            percent_type1_tick = true;
        } else if (arg == "--N" && i + 1 < argc) {
            N = check_stoi(argv[++i]);
            N_tick = true;
        } else if (arg == "--temp" && i + 1 < argc) {
            temp = stod(argv[++i]);
        }     
    }

    if (Lx_tick==false || Ly_tick==false || Lz_tick==false,
        dt_tick==false || T_tick==false || ic_tick==false) {
        cout << "Ensure --Lx, --Ly, --Lz, --dt and --T have a value and a --ic is given, type --help for more details" << endl;
        return 0;
    }

    if (ic_random_tick == true) {
        if (percent_type1_tick==false||N_tick==false) {
            cout << "Ensure --percent-type1 and --N have a value for --ic-random, type --help for more details" << endl;
            return 0;
        }

        for (int i = 0; i < N; i++) {
            if (i/N * 100 > percent_type1){
                int type = 0;
            }
            Particle p2(11.5, 8.7, 10.0, -0.5, 0.0, 0.0, type);
            box.addParticle(p2);
        }
    }

    box.printParticles();    

}

// Initial Condition 3
// Particle p1(8.5, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
// Particle p2(11.5, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
    
// box.addParticle(p1);
// box.addParticle(p2);

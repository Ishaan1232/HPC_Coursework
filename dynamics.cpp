#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

#include <boost/program_options.hpp>
namespace cl = boost::program_options;

#include "box.h"
#include "particle.h"

double rand_val(const double lower, const double upper) {
    return (double(rand())/RAND_MAX) * (upper - lower) + lower;
}

int main(int argc, char* argv[]) {

    double dt = 0.001;  // default time step
    double Lx = 20.0, Ly = 20.0, Lz = 20.0;  // default dimensions of box
    double percent_type1 = 10.0;  // default percentage of type 1
    double T;    // Final time
    double temp = -1.0; // if temp = -1.0, the temperature is not fixed
    int type = 1;
    bool ic_random = false;

    Box box(Lx, Ly, Lz); // Initialise the simulation box of the Box class

    try {
        // Define available command-line options
        cl::options_description desc("Allowed options");
        desc.add_options()
        ("help", "Print available options.")
        ("Lx", cl::value<double>()->default_value(20), "x length (Angstroms)")
        ("Ly", cl::value<double>()->default_value(20), "y length (Angstroms)")
        ("Lz", cl::value<double>()->default_value(20), "z length (Angstroms)")
        ("dt", cl::value<double>()->default_value(0.001), "Time-step")
        ("T", cl::value<double>(), "Final time")
        ("ic-one", "Initial condition: one stationary particle")
        ("ic-one-vel", "Initial condition: one moving particle")
        ("ic-two", "Initial condition: two bouncing particles")
        ("ic-two-pass1", "Initial condition: two passing particles")
        ("ic-two-pass2", "Initial condition: two passing particles close")
        ("ic-two-pass3", "Initial condition: two passing particles close, heavy")
        ("ic-random", "Initial condition: N random particles")
        ("percent-type1", cl::value<double>()->default_value(10), "Percentage of type 1 particles with random IC")
        ("N", cl::value<int>(), "Number of particles to spawn with random IC")
        ("temp", cl::value<double>(), "Temperature (degrees Kelvin)");

        // Parse command-line arguments
        cl::variables_map vm;
        cl::store(cl::parse_command_line(argc, argv, desc), vm);
        cl::notify(vm);

        // If help flag is provided, print options to terminal and exit
        if (vm.count("help")) {
            cout << desc << endl;
            return 0;
        }

        // Check if T has been given, end program if not
        if (!vm.count("T")) {
            cerr << "Error: Final time (--T) is required" << endl;
            return 1;
        }

        T = vm["T"].as<double>();
        
        // if --Lx arg is given, update from default
        if (vm.count("Lx")) {
            Lx = vm["Lx"].as<double>();
        }

        // if --Ly arg is given, update from default
        if (vm.count("Ly")) {
            Ly = vm["Ly"].as<double>();
        }

        // if --Lz arg is given, update from default
        if (vm.count("Lz")) {
            Lz = vm["Lz"].as<double>();
        }

        // if --dt arg is given, update from default
        if (vm.count("dt")) {
            dt = vm["dt"].as<double>();
        }

        // if temp arg is given, update from default
        if (vm.count("temp")) {
            temp = vm["temp"].as<double>();
            if (temp < 0) {
                cerr << "Error: The temperature temp must be greater than or equal to 0K" << endl;
                return 1;
            }
        }

        if (Lx <= 0 || Ly <= 0 || Lz <= 0 || dt <= 0 || T <= 0) {
            cerr << "Error: All lengths (Lx, Ly, Lz), the timestep dt and the final time T must be greater than 0" << endl; 
            return 1;
        }

        // Handle --ic-* flags (ensure only one is chosen)
        int ic_count = 0;
        string ic;
        vector<string> ic_options = {"ic-one", "ic-one-vel", "ic-two", "ic-two-pass1",
                                                "ic-two-pass2", "ic-two-pass3", "ic-random"};
        for (const auto& option : ic_options) {
            if (vm.count(option)) {
                ic_count++;
                ic = option;
            }
        }

        if (ic_count != 1) {
            cerr << "Error: You must specify exactly one initial condition (--ic).\n";
            return 1;
        }

        if (ic == "ic-one") {
            Particle p(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
            box.addParticle(p);
        } else if (ic == "ic-one-vel") {
            Particle p(10.0, 10.0, 10.0, 5.0, 2.0, 1.0, 0);
            box.addParticle(p);
        } else if (ic == "ic-two") {
            Particle p1(8.5, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
            Particle p2(11.5, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
            box.addParticle(p1);
            box.addParticle(p2);
        } else if (ic == "ic-two-pass1") {
            Particle p1(8.5, 11.5, 10.0, 0.5, 0.0, 0.0, 0);
            Particle p2(11.5, 8.5, 10.0, -0.5, 0.0, 0.0, 0);
            box.addParticle(p1);
            box.addParticle(p2);
        } else if (ic == "ic-two-pass2") {
            Particle p1(8.5, 11.3, 10.0, 0.5, 0.0, 0.0, 0);
            Particle p2(11.5, 8.7, 10.0, -0.5, 0.0, 0.0, 0);
            box.addParticle(p1);
            box.addParticle(p2);
        } else if (ic == "ic-two-pass3") {
            Particle p1(8.5, 11.3, 10.0, 0.5, 0.0, 0.0, 1);
            Particle p2(11.5, 8.7, 10.0, -0.5, 0.0, 0.0, 1);
            box.addParticle(p1);
            box.addParticle(p2);
        } else if (ic == "ic-random") {
            // ensure --N and --percent-type1 is provided
            if(!vm.count("N")) { 
                cerr << "Error: The argument --N is required when using --ic-random" << endl;
                return 1;
            }
            
            int N = vm["N"].as<int>();

            if (N <= 0) {
                cerr << "Error: The number of particles N must be greater than 0" << endl;
                return 0;
            }

            if (vm.count("percent-type1")) {
                percent_type1 = vm["percent-type1"].as<double>();
                if (percent_type1 < 0 || percent_type1 > 100) {
                    cerr << "Error: The percentage of type 1 particles must be between 0 and 100 inclusive" << endl;
                    return 0;
                }
            }

            for (int i = 0; i < N; i++) {
                if ((i+1.0)/N > percent_type1/100) {
                    type = 0;
                }

                bool remove = false;
                Particle p(rand_val(0, Lx), rand_val(0, Ly), rand_val(0, Lz), rand_val(-0.5, 0.5), rand_val(-0.5, 0.5), rand_val(-0.5, 0.5), type);
                box.addParticle(p);

                double diff[3];
                for (int j = 0; j < i; j++) {
                    if (box.findR(i, j, diff) < 0.5) {
                        remove = true;
                        break;
                    }
                }

                if (remove) {
                    box.removeLastParticle();
                    i--;
                } 
            }

            ic_random = true;
        }

    } catch (exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    box.runSimulation(Lx, Ly, Lz, dt, T, temp, ic_random);
}
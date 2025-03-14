/**
 * @file parse_cl.cpp
 * @brief Implements the parseCommandLine function
 * 
 * This file provides the implementation of the `parseCommandLine` function, which
 * extracts and validates user inputs using the Boost.ProgramOptions library.
 */

#include "parse_cl.h"

/** 
* This function utilizes the Boost.ProgramOptions library to handle user inputs,
* validate them appropriately, and return the parameters as a struct.
* 
* @param argc Number of command-line arguments.
* @param argv Array of all command-line arguments.
* @return SimulationParameters Struct containing simulation parameters.
*/
SimulationParameters parseCommandLine(int argc, char* argv[]) {
    SimulationParameters param;        
    param.ic_random = false;
    param.temp = -1;                                                                                    // Initiliase temp to -1                         

    try {
        // Define available command-line options
        cl::options_description desc("Allowed options");
        desc.add_options()
            ("help", "Print available options.")
            ("Lx", cl::value<double>()->default_value(20), "x length (Angstroms)")                      // Set as default
            ("Ly", cl::value<double>()->default_value(20), "y length (Angstroms)")                      // Set as default
            ("Lz", cl::value<double>()->default_value(20), "z length (Angstroms)")                      // Set as default
            ("dt", cl::value<double>()->default_value(0.001), "Time-step")                              // Set as default
            ("T", cl::value<double>(), "Final time")
            ("temp", cl::value<double>(), "Temperature (Kelvin)")                    
            ("percent-type1", cl::value<double>()->default_value(10), "Percentage of type 1 particles") // Set as default
            ("N", cl::value<int>(), "Number of particles for random initialization")
            ("ic-one", "Initial condition: one stationary particle")
            ("ic-one-vel", "Initial condition: one moving particle")
            ("ic-two", "Initial condition: two bouncing particles")
            ("ic-two-pass1", "Initial condition: two passing particles")
            ("ic-two-pass2", "Initial condition: two passing particles close")
            ("ic-two-pass3", "Initial condition: two passing particles close, heavy")
            ("ic-random", "Initial condition: N random particles");

        // Parse command-line arguments
        cl::variables_map vm;
        cl::store(cl::parse_command_line(argc, argv, desc), vm);
        cl::notify(vm);

        // Print description to terminal
        if (vm.count("help")) {
            cout << desc << endl;
            exit(0);
        }

        // Check if mandatory T is given
        if (!vm.count("T")) {
            throw runtime_error("Error: Final time (--T) is required");
        }

        param.T = vm["T"].as<double>();
        param.Lx = vm["Lx"].as<double>();
        param.Ly = vm["Ly"].as<double>();
        param.Lz = vm["Lz"].as<double>();
        param.dt = vm["dt"].as<double>();
        
        // Ensure if temperature given, it is greater than 0K
        if (vm.count("temp")) {
            param.temp = vm["temp"].as<double>();
            if (param.temp < 0) {
                throw runtime_error("Error: The temperature must be >= 0K");
            }
        }
        
        // Ensure all times and lengths are greater than 0
        if (param.Lx <= 0 || param.Ly <= 0 || param.Lz <= 0 || param.dt <= 0 || param.T <= 0) {
            throw runtime_error("Error: All lengths (Lx, Ly, Lz), dt, and T must be greater than 0");
        }

        int ic_count = 0;
        vector<string> ic_options = {                     // Accepted initical condition arguments
            "ic-one", "ic-one-vel", "ic-two", "ic-two-pass1",
            "ic-two-pass2", "ic-two-pass3", "ic-random"
        };

        // Identify initial condition and count number of initial conditions given
        for (const auto& option : ic_options) {
            if (vm.count(option)) {
                ic_count++;
                param.ic = option;
            }
        }

        // Ensure only 1 initial condition given
        if (ic_count != 1) {
            throw runtime_error("Error: You must specify exactly one initial condition (--ic)");
        }

        // Check if N is given and both N and percent of type 1 particles are in a valid range if initial condition is random particles
        if (param.ic == "ic-random") {
            if (!vm.count("N")) {
                throw runtime_error("Error: The argument --N is required when using --ic-random");
            }
            param.N = vm["N"].as<int>();
            if (param.N <= 0) {
                throw runtime_error("Error: The number of particles N must be > 0");
            }
            if (vm.count("percent-type1")) {
                param.percent_type1 = vm["percent-type1"].as<double>();
                if (param.percent_type1 < 0 || param.percent_type1 > 100) {
                    throw runtime_error("Error: The percentage of type 1 particles must be between 0 and 100");
                }
            }
        }

    } catch (exception& e) {
        cerr << e.what() << endl;
        exit(1);
    }

    return param;
}

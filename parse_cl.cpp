#include "parse_cl.h"
#include <iostream>
#include <vector>
#include <boost/program_options.hpp>

namespace cl = boost::program_options;

SimulationParameters parseCommandLine(int argc, char* argv[]) {
    SimulationParameters param;
    param.dt = 0.001;
    param.Lx = param.Ly = param.Lz = 20.0;
    param.percent_type1 = 10.0;
    param.temp = -1.0;
    param.type = 1;
    param.ic_random = false;
    param.N = 0;

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
            ("temp", cl::value<double>(), "Temperature (Kelvin)")
            ("percent-type1", cl::value<double>()->default_value(10), "Percentage of type 1 particles")
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

        if (vm.count("help")) {
            std::cout << desc << std::endl;
            exit(0);
        }

        if (!vm.count("T")) {
            throw std::runtime_error("Error: Final time (--T) is required");
        }

        param.T = vm["T"].as<double>();
        param.Lx = vm["Lx"].as<double>();
        param.Ly = vm["Ly"].as<double>();
        param.Lz = vm["Lz"].as<double>();
        param.dt = vm["dt"].as<double>();

        if (vm.count("temp")) {
            param.temp = vm["temp"].as<double>();
            if (param.temp < 0) {
                throw std::runtime_error("Error: The temperature must be >= 0K");
            }
        }

        if (param.Lx <= 0 || param.Ly <= 0 || param.Lz <= 0 || param.dt <= 0 || param.T <= 0) {
            throw std::runtime_error("Error: All lengths (Lx, Ly, Lz), dt, and T must be greater than 0");
        }

        int ic_count = 0;
        std::vector<std::string> ic_options = {
            "ic-one", "ic-one-vel", "ic-two", "ic-two-pass1",
            "ic-two-pass2", "ic-two-pass3", "ic-random"
        };

        for (const auto& option : ic_options) {
            if (vm.count(option)) {
                ic_count++;
                param.ic = option;
            }
        }

        if (ic_count != 1) {
            throw std::runtime_error("Error: You must specify exactly one initial condition (--ic)");
        }

        if (param.ic == "ic-random") {
            if (!vm.count("N")) {
                throw std::runtime_error("Error: The argument --N is required when using --ic-random");
            }
            param.N = vm["N"].as<int>();
            if (param.N <= 0) {
                throw std::runtime_error("Error: The number of particles N must be > 0");
            }
        }

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    return param;
}

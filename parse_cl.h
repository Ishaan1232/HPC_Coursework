/**
 * @file particle.h
 * @brief Defines the SimulationParameters struct for the simulation's parameters.
 * 
 * The file declares the SimulationParameters struct and the function parseCommandLine
 * to extract and validate user inputs using Boost.ProgramOptions.
 */

#ifndef PARSE_CL_H
#define PARSE_CL_H

#include <string>
#include <boost/program_options.hpp>

/**
 * @struct SimulationParameters 
 * @brief Stores simulation parameters parsed from the command line.
 * 
 * This struct contains parameters for configuring the simulation,
 * including domain size, time step, temperature, and the initial condition.
 */
struct SimulationParameters {   
    double dt;                  ///< Time step size (default = 0.001)
    double Lx, Ly, Lz;          ///< Dimensions of the box (default = 20 x 20 x 20)
    double T;                   ///< Time to run simulation 
    double temp;                ///< Temperature to be fixed in Kelvin (-1 if not fixed) 
    double percent_type1;       ///< Percent of type 1 particles for random initial condition (default = 10%)
    bool ic_random;             ///< Boolean indicating if initial condition is random particles
    std::string ic;             ///< Initial condition 
    int N;                      ///< Number of particle for random initial condtion
};

/// @brief Parses command-line arguments to get simulation parameters
SimulationParameters parseCommandLine(int argc, char* argv[]);

#endif // PARSE_CL_H

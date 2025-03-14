#ifndef PARSE_CL_H
#define PARSE_CL_H

#include <string>
#include <boost/program_options.hpp>

struct SimulationParameters {
    double dt;
    double Lx, Ly, Lz;
    double T;
    double temp;
    double percent_type1;
    int type;
    bool ic_random;
    std::string ic;
    int N;
};

SimulationParameters parseCommandLine(int argc, char* argv[]);

#endif // PARSE_CL_H

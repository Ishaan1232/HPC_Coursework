/**
 * @file box.h
 * @brief Defines the Box class.
 * 
 * This header declares the Box class, which manages particles and their interactions
 * within a 3D simulation environment.
 */

#ifndef BOX_H
#define BOX_H// header guard to prevent being included in multiple files

#include <iostream>
#include <cmath> 
#include <vector>
#include <fstream> 
#include <iomanip>
#include <string>
#include "particle.h"

/**
 * @class Box
 * @brief Represents the simulation box 
 */
class Box {
    private:
        const double Lx, Ly, Lz;   ///< Box dimensions in the x, y and z directions
        vector<Particle> particles;  ///< Vector to hold particles
        int N = 0;                  ///< Number of particles

    public:
        /// @brief Default constructor
        Box();
        /// @brief Construct the test box with the given dimensions
        Box(const double x_length, const double y_length, const double z_length);  
        /// @brief Default destructor
        ~Box() = default;
        /// @brief Adds a particle to the box and ensures there is a minimum separation.
        bool addParticle(Particle& p); 
        /// @brief Calculate the force on a particle exerted by all other particles
        void calculateF_i(Particle& p_i, int i);   
        /// @brief Return the kinetic energy of the system
        double systemKE();
        /// @brief Run the simulation once all particles are added successfully
        void runSimulation(double dt, double T, double temp, bool ic_random, string ic); 
};

#endif // BOX_H
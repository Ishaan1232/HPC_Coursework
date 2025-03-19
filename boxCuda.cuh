/**
 * @file box.h
 * @brief Defines the Box class for CUDA
 * 
 * This header declares the Box class, which manages particles and their interactions
 * within a 3D simulation environment using CUDA.
 */

#ifndef BOX_H
#define BOX_H// header guard to prevent being included in multiple files

#include <cmath> 
#include <vector>
#include <fstream> 
#include <iomanip>
#include <string>
#include <array>
#include "particle.h"
/**
 * @class Box
 * @brief Represents the simulation box 
 */
class Box {
    private:
        const double Lx, Ly, Lz;   ///< Box dimensions in the x, y and z directions
        vector<Particle> particles;  ///< Vector to hold particles
        int N;                  ///< Number of particles

        double* host_particles; ///< Host array for particle info (x1, y1, z1, vx1, vx2, vx3, Fx1, Fx2, Fx3, mass1, x2, y2, z2, ...)
        double* gpu_particles; ///< GPU array for particle info (x1, y1, z1, vx1, vx2, vx3, Fx1, Fx2, Fx3, mass1, x2, y2, z2, ...)


    public:
        /// @brief Default constructor
        Box();
        /// @brief Construct the test box with the given dimensions
        Box(const double x_length, const double y_length, const double z_length);  
        /// @brief Destructor
        ~Box();
        /// @brief Adds a particle to the box and ensures there is a minimum separation.
        bool addParticle(Particle& p); 
        /// @brief Allocate memory for GPU array and populates host array
        void createArrays(vector<Particle>& particles);
        /// @brief Run the simulation once all particles are added successfully
        void runSimulation(double dt, double T, double temp, bool ic_random, string ic); 
};

#endif // BOX_H
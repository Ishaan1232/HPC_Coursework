/**
 * @file particle.h
 * @brief Defines the Particle class for molecular dynamics simulation.
 * 
 * The Particle class represents an individual particle in the simulation.
 * It includes properties such as position, velocity, force, and mass,
 * as well as methods for updating its state.
 */

#ifndef PARTICLE_H
#define PARTICLE_H// header guard to prevent being included in multiple files

#include <iostream>
#include <cmath> 
#include <array>
#include <fstream> 
#include <iomanip>
using namespace std;

/**
 * @class Particle
 * @brief Represents a particle in the box
 */
class Particle {
    private:
        array<double, 3> v;           ///< Velocity of the particle
        double mass;         ///< Mass of the particle
        /// @brief Checks and applies boundary conditions to ensure the particle stays within the domain.
        void applyBC(double& pos, double& vel, double L);

    public:
        int type;            ///< Type of the particle
        array<double, 3> r;           ///< Position of the particle
        array<double, 3> F;           ///< Force of the particle

        /// @brief Default constructor
        Particle();
        /// @brief Constructor for particles with given position, velocity and type(mass)
        Particle(double x_i, double y_i, double z_i, double vx_i, double vy_i, double vz_i, int type_i);
        /// @brief Copy constructor
        Particle(const Particle& p);    
        /// @brief Default deconstructor
        ~Particle() = default;
        /// @brief Updates the particle's position
        void updatePosition(double dt);
        /// @brief Updates the particle's velocity
        void updateVelocity(double dt, double Lx, double Ly, double Lz);
        /// @brief Calculates the particle's kinetic energy
        double particleKE();
        /// @brief Scales the particle's velocity for constant temperature
        void scaleTemp(double lambda);

        /**
         * @brief Get the velocity of the particle
         * 
         * @return The array holding the particle's velocity
         */
        const array<double, 3>& get_v() {return v;}

        /**
         * @brief Get the mass of the particle
         * 
         * @return mass of the particle
         */
        double get_mass() {return mass;}
};

#endif // PARTICLE_H
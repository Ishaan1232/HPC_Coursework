#ifndef PARTICLE_H
#define PARTICLE_H// header guard to prevent being included in multiple files

#include <iostream>
#include <cmath> 
#include <tuple>
#include <fstream> 
#include <iomanip>
using namespace std;

/**
 * @class Particle
 * @brief Represents a particle in the box
 */
class Particle {
    private:
        double* v;           ///< Velocity of the particle
        double mass;         ///< Mass of the particle
        /// @brief Checks and applies boundary conditions to ensure the particle stays within the domain.
        void applyBC(double* pos, double* vel, double L);

    public:
        double* r;           ///< Position of the particle
        double* F;           ///< Force of the particle
        int type;            ///< Type of the particle

        /// @brief Default constructor
        Particle();
        /// @brief Constructor for particles with given position, velocity and type(mass)
        Particle(double x_i, double y_i, double z_i, double vx_i, double vy_i, double vz_i, int type_i);
        /// @brief Copy constructor
        Particle(const Particle& p);    
        /// @brief Deconstructor
        ~Particle();
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
         * @return Pointer to the velocity of the particle
         */
        const double* get_v() {return v;}
};

#endif // PARTICLE_H
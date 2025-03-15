#include "box.h"
#include "particle.h"

#define BOOST_TEST_MODULE Box_Particle
#include <boost/test/included/unit_test.hpp>

const double tol = 1e-5;

// Test particle is initialised corectly
BOOST_AUTO_TEST_CASE(InitiliaseParticle) { 
    Particle p(10, 10, 10, 1, 1, 1, 0);

    BOOST_CHECK_CLOSE(p.r[0], 10, tol);
    BOOST_CHECK_CLOSE(p.r[1], 10, tol);
    BOOST_CHECK_CLOSE(p.r[2], 10, tol);

    BOOST_CHECK_CLOSE(p.get_v()[0], 1, tol);
    BOOST_CHECK_CLOSE(p.get_v()[1], 1, tol);
    BOOST_CHECK_CLOSE(p.get_v()[2], 1, tol);

    BOOST_CHECK_CLOSE(p.F[0], 0, tol);
    BOOST_CHECK_CLOSE(p.F[1], 0, tol);
    BOOST_CHECK_CLOSE(p.F[2], 0, tol);

    BOOST_CHECK_EQUAL(p.type, 0);
}

// // Test box is correctly intialised
// BOOST_AUTO_TEST_CASE(InitialiseBox) {
//     Box box(20, 20, 20);
//     Particle p1(5, 5, 5, 0, 0, 0, 0);
//     Particle p2(15, 15, 15, 0, 0, 0, 1);
//     box.addParticle(p1);
//     box.addParticle(p2);

//     BOOST_CHECK_CLOSE(box.get_Lx(), 20, tol);
//     BOOST_CHECK_CLOSE(box.get_Ly(), 20, tol);
//     BOOST_CHECK_CLOSE(box.get_Lz(), 20, tol);

//     BOOST_CHECK_EQUAL(box.get_N(), 2);
// }

// Test position update
BOOST_AUTO_TEST_CASE(ParticlePositionUpdate) {
    Particle p(10, 10, 10, 5, 2, 1, 0);
    p.updatePosition(0.001);

    BOOST_CHECK_CLOSE(p.r[0], 10.005, tol);
    BOOST_CHECK_CLOSE(p.r[1], 10.002, tol);
    BOOST_CHECK_CLOSE(p.r[2], 10.001, tol);
}

// Test velocity update
BOOST_AUTO_TEST_CASE(ParticleVelocityUpdate) {
    Particle p(10, 10, 10, 0, 0, 0, 0);
    p.F = {1, 1, 1};

    p.updateVelocity(0.001, 20, 20, 20);

    BOOST_CHECK_CLOSE(p.get_v()[0], 0.001, tol);
    BOOST_CHECK_CLOSE(p.get_v()[1], 0.001, tol);
    BOOST_CHECK_CLOSE(p.get_v()[2], 0.001, tol);
}

// Test Boundary Conditions
BOOST_AUTO_TEST_CASE(ParticleBoundaryCondition) {
    Particle p(19.9, 10, 10, 5, 0, 0, 0);   // Moving towards wall with 5m/s in x-direction

    p.updatePosition(0.1);                          // Update position with dt = 0.1s
    p.updateVelocity(0.1, 20, 20, 20);     // update velocity with dt = 0.1s

    BOOST_CHECK_CLOSE(p.r[0], 19.6, tol);
    BOOST_CHECK_CLOSE(p.r[1], 10, tol);
    BOOST_CHECK_CLOSE(p.r[2], 10, tol);

    BOOST_CHECK_CLOSE(p.get_v()[0], -5, tol);
    BOOST_CHECK_CLOSE(p.get_v()[1], 0, tol);
    BOOST_CHECK_CLOSE(p.get_v()[2], 0, tol);
}

// Test Kinetic Energy Calculation
BOOST_AUTO_TEST_CASE(ParticleKineticEnergy) {
    Particle p(10, 10, 10, 3, 4, 0, 0);
    double expected_ke = 0.5 * 1.0 * (3 * 3 + 4 * 4);  
    
    BOOST_CHECK_CLOSE(p.particleKE(), expected_ke, tol);
}

// Test Constant Temperature
BOOST_AUTO_TEST_CASE(ConstantTemperature) {
    Particle p(10, 10, 10, 0.5, 0, 0, 0);
    double temp = 30;

    double E_1 = p.particleKE();
    double lambda_1 = sqrt((temp * 1.5 * 0.8314459920816467)/E_1);
    p.scaleTemp(lambda_1);
    double E_init = p.particleKE();

    p.updatePosition(0.001);
    p.F = {1.0, 1.0, 1.0};
    p.updateVelocity(0.001, 20, 20, 20);

    double E_2 = p.particleKE();
    double lambda_2 = sqrt((temp * 1.5 * 0.8314459920816467)/E_2);
    p.scaleTemp(lambda_2);
    double E_after = p.particleKE();
    
    BOOST_CHECK_CLOSE(E_init - E_after, 0.0, tol);
}
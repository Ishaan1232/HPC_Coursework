#include "box.h"
#include "particle.h"
#include <string>
#include <vector>
using namespace std;

#define BOOST_TEST_MODULE Box_Particle_Par
#include <boost/test/included/unit_test.hpp>

const double tol = 1e-2;

// test --ic-one
BOOST_AUTO_TEST_CASE(Test1) {
    Box box(20, 20, 20);
    Particle p(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
    box.addParticle(p);

    double dt = 0.001;
    double T = 10;
    double temp = -1;
    bool ic_random = false;
    string ic = "ic-one";
    box.runSimulation(dt, T, temp, ic_random, ic);

    const vector<Particle>& particles = box.get_particles();

    BOOST_CHECK_CLOSE(particles[0].r[0], 10.0, tol);
    BOOST_CHECK_CLOSE(particles[0].r[1], 10.0, tol);
    BOOST_CHECK_CLOSE(particles[0].r[2], 10.0, tol);
}

// test --ic-one-vel
BOOST_AUTO_TEST_CASE(Test2) {
    Box box(20, 20, 20);
    Particle p(10.0, 10.0, 10.0, 5.0, 2.0, 1.0, 0);
    box.addParticle(p);

    double dt = 0.001;
    double T = 20;
    double temp = -1;
    bool ic_random = false;
    string ic = "ic-one-vel";
    box.runSimulation(dt, T, temp, ic_random, ic);

    const vector<Particle>& particles = box.get_particles();

    BOOST_CHECK_CLOSE(particles[0].r[0], 10.0, tol);
    BOOST_CHECK_CLOSE(particles[0].r[1], 10.0, tol);
    BOOST_CHECK_CLOSE(particles[0].r[2], 10.0, tol);
}

// test --ic-two
BOOST_AUTO_TEST_CASE(Test3) {
    Box box(20, 20, 20);
    Particle p1(8.5, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
    Particle p2(11.5, 10.0, 10.0, 0.0, 0.0, 0.0, 0);
    box.addParticle(p1);
    box.addParticle(p2);

    double dt = 0.001;
    double T = 50;
    double temp = -1;
    bool ic_random = false;
    string ic = "ic-two";
    box.runSimulation(dt, T, temp, ic_random, ic);

    const vector<Particle>& particles = box.get_particles();

    BOOST_CHECK_CLOSE(particles[0].r[0], 8.50767, tol);
    BOOST_CHECK_CLOSE(particles[0].r[1], 10.0, tol);
    BOOST_CHECK_CLOSE(particles[0].r[2], 10.0, tol);

    BOOST_CHECK_CLOSE(particles[1].r[0], 11.4923, tol);
    BOOST_CHECK_CLOSE(particles[1].r[1], 10.0, tol);
    BOOST_CHECK_CLOSE(particles[1].r[2], 10.0, tol);
}

// test --ic-two-pass1
BOOST_AUTO_TEST_CASE(Test4) {
    Box box(20, 20, 20);
    Particle p1(8.5, 11.5, 10.0, 0.5, 0.0, 0.0, 0);
    Particle p2(11.5, 8.5, 10.0, -0.5, 0.0, 0.0, 0);
    box.addParticle(p1);
    box.addParticle(p2);

    double dt = 0.001;
    double T = 50;
    double temp = -1;
    bool ic_random = false;
    string ic = "ic-two-pass1";
    box.runSimulation(dt, T, temp, ic_random, ic);

    const vector<Particle>& particles = box.get_particles();

    BOOST_CHECK_CLOSE(particles[0].r[0], 7.24856, tol);
    BOOST_CHECK_CLOSE(particles[0].r[1], 5.71575, tol);
    BOOST_CHECK_CLOSE(particles[0].r[2], 10.0, tol);

    BOOST_CHECK_CLOSE(particles[1].r[0], 12.7514, tol);
    BOOST_CHECK_CLOSE(particles[1].r[1], 14.2842, tol);
    BOOST_CHECK_CLOSE(particles[1].r[2], 10.0, tol);
}

// test --ic-two-pass2
BOOST_AUTO_TEST_CASE(Test5) {
    Box box(20, 20, 20);
    Particle p1(8.5, 11.3, 10.0, 0.5, 0.0, 0.0, 0);
    Particle p2(11.5, 8.7, 10.0, -0.5, 0.0, 0.0, 0);
    box.addParticle(p1);
    box.addParticle(p2);

    double dt = 0.001;
    double T = 50;
    double temp = -1;
    bool ic_random = false;
    string ic = "ic-two-pass2";
    box.runSimulation(dt, T, temp, ic_random, ic);

    const vector<Particle>& particles = box.get_particles();

    BOOST_CHECK_CLOSE(particles[0].r[0], 12.737, tol);
    BOOST_CHECK_CLOSE(particles[0].r[1], 18.1321, tol);
    BOOST_CHECK_CLOSE(particles[0].r[2], 10.0, tol);

    BOOST_CHECK_CLOSE(particles[1].r[0], 7.26299, tol);
    BOOST_CHECK_CLOSE(particles[1].r[1], 1.86789, tol);
    BOOST_CHECK_CLOSE(particles[1].r[2], 10.0, tol);
}

// test --ic-two-pass2
BOOST_AUTO_TEST_CASE(Test6) {
    Box box(20, 20, 20);
    Particle p1(8.5, 11.3, 10.0, 0.5, 0.0, 0.0, 1);
    Particle p2(11.5, 8.7, 10.0, -0.5, 0.0, 0.0, 1);
    box.addParticle(p1);
    box.addParticle(p2);

    double dt = 0.001;
    double T = 50;
    double temp = -1;
    bool ic_random = false;
    string ic = "ic-two-pass3";
    box.runSimulation(dt, T, temp, ic_random, ic);

    const vector<Particle>& particles = box.get_particles();

    BOOST_CHECK_CLOSE(particles[0].r[0], 10.5897, tol);
    BOOST_CHECK_CLOSE(particles[0].r[1], 8.11567, tol);
    BOOST_CHECK_CLOSE(particles[0].r[2], 10.0, tol);

    BOOST_CHECK_CLOSE(particles[1].r[0], 9.41029, tol);
    BOOST_CHECK_CLOSE(particles[1].r[1], 11.8843, tol);
    BOOST_CHECK_CLOSE(particles[1].r[2], 10.0, tol);
}
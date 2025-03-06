#include "box.h" 

Box::Box(double x, double y, double z)
    : Lx(x), Ly(y), Lz(z) {
        // Nothing to do
    }

double Box::findR(int i, int j, double diff[3]) {
    cblas_dcopy(3, particles[i].get_r().data(), 1, diff, 1);
    cblas_daxpy(3, -1.0, particles[j].get_r().data(), 1, diff, 1);
    return cblas_dnrm2(3, diff, 1);
}

void Box::calculateF_i(int i) {
    double eps, sig, r_ij, dphi_dx;
    double diff[3];
    vector<double> F_i = particles[i].get_F();
    cblas_dscal(3, 0, F_i.data(), 1);
    for (size_t j = 0; j < particles.size(); j++) {
        if (i != j) {
            if (particles[i].get_type() == particles[j].get_type()) {
                if (particles[i].get_type() ==  0) {
                    eps = 3.0; 
                    sig = 1.0;
                } else {
                    eps = 60.0;
                    sig = 3.0;
                }
            } else {
                eps = 15.0;
                sig = 2.0;
            }

            r_ij = findR(i, j, diff);
            dphi_dx = -24 * eps * (2*pow(sig/r_ij, 12) - pow(sig/r_ij, 6)) / (r_ij*r_ij);

            cblas_daxpy(3, -dphi_dx, diff, 1, F_i.data(), 1);
        }
    }
    particles[i].set_F(F_i);
}

void Box::addParticle(const Particle& p) {
    particles.push_back(p);
}

void Box::removeLastParticle() {
    particles.pop_back();
}

void Box::runSimulation(double dt, double T, double temp, bool ic_random, const string& ic) {
    string particle_file = "Particle_Data/Data/" + ic + ".txt";
    string KE_file = "KE_Data/Data/" + ic + ".txt";
    ofstream particleData(particle_file, ios::out | ios::trunc);
    ofstream KEData(KE_file, ios::out | ios::trunc);

    int N = particles.size();

    for (double t = 0; t < T + dt; t += dt) {
        double E = 0.0;
        for (int i = 0; i < N; i++) {
            calculateF_i(i);                                   // force at time t, starting at t = 0.0
            E += particles[i].particleKE();

            if (fmod(t, 0.1) < dt) {
                if (!ic_random) {
                    particleData << setw(7) << round(t * 10) / 10
                                << setw(7) << i + 1 
                                << setw(15) << particles[i].get_r()[0]
                                << setw(15) << particles[i].get_r()[1]
                                << setw(15) << particles[i].get_r()[2]
                                << setw(15) << particles[i].get_v()[0]
                                << setw(15) << particles[i].get_v()[1]
                                << setw(15) << particles[i].get_v()[2] << endl;
                }
            }
        }

        if (fmod(t, 0.1) < dt) {
            KEData  << setw(7) << round(t * 10) / 10 << setw(15) << E << endl;
        }

        for (int i = 0; i < N; i++) {
            particles[i].updatePosition(dt, Lx, Ly, Lz, temp, E);    // particles at time t + dt
        }

    }


    particleData.close();
    KEData.close();
}
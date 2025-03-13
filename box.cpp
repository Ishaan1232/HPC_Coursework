#include "box.h"

Box::Box(const double x_length, const double y_length, const double z_length) 
    : Lx(x_length), Ly(y_length), Lz(z_length) {

}

bool Box::addParticle(Particle& p) {
    particles.push_back(p);

    for (int j = 0; j < N; j++) {

        double d, R2 = 0.0;
        for (int m = 0; m < 3; m++) {
            d = p.r[m] - particles[j].r[m];
            R2 += d*d;
        }
        if (R2 < 0.25) {
            particles.pop_back();
            return false;
        }
        
    }
    N++;
    return true;
}


void Box::calculateF_i(int i, bool ic_random) {
    double eps, sig, r_ij2, dphi_dx, sig_rij, inv_rij2;
    double diff[3];
    double F_i[3] = {0.0, 0.0, 0.0};
    Particle& p_i = particles[i];
    for (int j = 0; j < N; j++) {
        if (i != j) {
            Particle& p_j = particles[j];
            if (p_i.type == p_j.type) {
                if (p_i.type == 0) {
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

            for (int m = 0; m < 3; m++) {
                diff[m] = p_i.r[m] - p_j.r[m];
            }

            r_ij2 = diff[0]*diff[0] + diff[1]*diff[1] + diff[2]*diff[2];

            if (ic_random) {
                double cutoff = 2.5 * sig;
                if (r_ij2 >= cutoff * cutoff) {
                    continue;
                }
            }

            inv_rij2 = 1.0/(r_ij2);
            sig_rij = (sig*sig*sig*sig*sig*sig) * inv_rij2*inv_rij2*inv_rij2;  // (sigma/r)^6 (avoids pow)
            dphi_dx = -24 * eps * sig_rij * (2 * sig_rij  - 1) * inv_rij2;

            for (int m = 0; m < 3; m++) {
                F_i[m] -= dphi_dx * diff[m];
            }
        }
    }
    p_i.set_F(F_i);
}

double Box::systemKE() {
    double E = 0.0;
    for (int i = 0; i < N; i++) {
        E += particles[i].particleKE();
    }
    return E;
}

void Box::runSimulation(double dt, double T, double temp, bool ic_random, string ic) {
    string particle_file = "Particle_Data/Data/" + ic + ".txt";
    string KE_file = "KE_Data/Data/" + ic + ".txt";
    ofstream particleData(particle_file, ios::out | ios::trunc);
    ofstream KEData(KE_file, ios::out | ios::trunc);

    double E = 0.0, lambda;
    if (temp != -1) {
        E = systemKE();
        lambda = sqrt((temp * 1.5 * 0.8314459920816467)/E);
        for (int i = 0; i < N; i++) {
            particles[i].scaleTemp(lambda);
        }
    }

    for (double t = 0; t < T + dt; t += dt) {
        for (int i = 0; i < N; i++) {
            if (!ic_random) {
                if (fmod(t, 0.1) < dt) {
                    particleData << setw(7) << round(t * 10) / 10
                                << setw(7) << i + 1 
                                << setw(15) << particles[i].r[0]
                                << setw(15) << particles[i].r[1]
                                << setw(15) << particles[i].r[2]
                                << setw(15) << particles[i].get_v()[0]
                                << setw(15) << particles[i].get_v()[1]
                                << setw(15) << particles[i].get_v()[2] << endl;
                }
            }
            particles[i].updatePosition(dt);            // update position first
        }

        for (int i = 0; i < N; i++) {
            calculateF_i(i, ic_random);                                   // force
        }

        E = systemKE();
        lambda = sqrt((temp * 1.5 * 0.8314459920816467)/E);
        if (fmod(t, 0.1) < dt) {
            KEData  << setw(7) << round(t * 10) / 10 << setw(15) << E << endl;
        }

        for (int i = 0; i < N; i++) {
            Particle& p = particles[i];
            p.updateVelocity(dt, Lx, Ly, Lz);
            if (temp != -1) {
                p.scaleTemp(lambda);
            }    
        }        
    }

    particleData.close();
    KEData.close();
}
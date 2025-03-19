/**
 * @file box.cpp
 * @brief Implements the Box class methods.
 * 
 * This file provides the implementation of the Box class, including
 * methods for particle management, force calculations, and simulation execution.
 */

#include "boxCuda.cuh"

/**
 * Initializes the box with given dimensions
 * 
 * @param x_length Length of the box in the x-direction.
 * @param y_length Length of the box in the y-direction.
 * @param z_length Length of the box in the z-direction.
 */
Box::Box(const double x_length, const double y_length, const double z_length) 
    : Lx(x_length), Ly(y_length), Lz(z_length), N(0), host_particles(nullptr), gpu_particles(nullptr) {
}

/**
 * Dellocate memory for the host and GPU arrays
 */
Box::~Box() {
    delete[] host_particles;
    cudaFree(gpu_particles);
}

/**
 * Adds the given particle to the box.
 * Uses /f$ R^2 = x_{ij}^2 + y_{ij}^2 + z_{ij}^2 /f$ where /f$ a_{ij} = a_i - a_j /f$.
 * If /f$ R^2 < 0.25 /f$, the particle is removed.
 * 
 * @param p The particle to be added to the box.
 * @return 'true' if the particle was added sucessfully, 'false' if not.
 */
bool Box::addParticle(Particle& p) {
    particles.push_back(p);             // append particle to vector

    for (int j = 0; j < N; j++) {       // iterate through every other particle
        Particle& p_j = particles[j];
        double d, R2 = 0.0;
        for (int m = 0; m < 3; m++) {
            d = p.r[m] - p_j.r[m];
            R2 += d*d;                  
        }
        if (R2 < 0.25) {
            particles.pop_back();       // remove particle in last position if R^2 < 0.25
            return false;
        }
    }
    N++;                                // Increment number of particles if added successfully
    return true;
}

/**
 * @brief CUDA Kernel to calculate the force on a particle exerted by all other particles
 * 
 * Uses /f$ \mathbf{F}_i = - \nabla \sum_{\substack{j \neq i}}^{N} \phi_{ij}./f$ to calculate
 * the force on particle i, where \frac{d \phi_{ij}}{dx} = -24 \epsilon x_{ij} \frac{\sigma^6}{r_{ij}^8} \left( \frac{2\sigma^6}{r_{ij}^6} - 1 \right).
 * Using Newton's Law, the force exerted on p_i by p_j is equal but opposite to the force exerted on p_j by p_i.
 * Therefore, the total force is calculated on p_i, but also the force exerted by p_i on other particles.
 * 
 * @param gpu_particles The gpu array to be used
 * @param N The total number of particles
 */
__global__ 
void calculateF_i(double* gpu_particles, int N) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;   // calculate array index
    if (i < N) {                                    // Ensure thread only executes if it is less than N
        double Fx = 0.0, Fy = 0.0, Fz = 0.0;
        double eps, sig, dx, dy, dz, r_ij2, dphi_dx, sig_rij, inv_rij2;

        for (int j = 0; j < N; j++) {
            if (i != j) {       // skip identical particle index

                if (gpu_particles[10 * i + 9] == gpu_particles[10 * j + 9]) {         // Find the parameters based on the two particles
                    if (gpu_particles[10 * i + 9] == 1.0) {
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

                dx = gpu_particles[10 * i] - gpu_particles[10 * j];
                dy = gpu_particles[10 * i + 1] - gpu_particles[10 * j + 1];
                dz = gpu_particles[10 * i + 2] - gpu_particles[10 * j + 2];

                r_ij2 = dx * dx + dy * dy + dz * dz;
                inv_rij2 = 1.0 / r_ij2;                                                             // Calculate 1/r^2
                sig_rij = (sig * sig * sig * sig * sig * sig) * inv_rij2 * inv_rij2 * inv_rij2;     // Calcualte (sigma/r)^6
                dphi_dx = 24.0 * eps * sig_rij * (2 * sig_rij - 1) * inv_rij2;                       // Calculate the constant in the force equation

                Fx += dphi_dx * dx;
                Fy += dphi_dx * dy;
                Fz += dphi_dx * dz;
            }
        }
        gpu_particles[10 * i + 6] = Fx;
        gpu_particles[10 * i + 7] = Fy;
        gpu_particles[10 * i + 8] = Fz;
    }
}

/**
 * @brief CUDA Kernel to update positions
 * 
 * @param gpu_particles gpu array 
 * @param dt time step
 * @param N number of particles
 */
__global__ 
void updatePositions(double *gpu_particles, double dt, int N) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= N) return;

    gpu_particles[10 * i]     += dt * gpu_particles[10 * i + 3];
    gpu_particles[10 * i + 1] += dt * gpu_particles[10 * i + 4];
    gpu_particles[10 * i + 2] += dt * gpu_particles[10 * i + 5];
}

/**
 * @brief CUDA Kernel to update velocities and apply boundary conditions
 * 
 * Uses the equation \f$ v = v + (F/m) \cdot dt \f$.
 * Also applies boundary conditions to prevent particles from exiting the simulation domain.
 * 
 * @param gpu_particles gpu array 
 * @param dt time step
 * @param N number of particles
 * @param Lx Box dimension in the x-direction.
 * @param Ly Box dimension in the y-direction.
 * @param Lz Box dimension in the z-direction.
 */
__global__ 
void updateVelocities(double *gpu_particles, double dt, int N, double Lx, double Ly, double Lz) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < N) {
        double mass = gpu_particles[10 * i + 9];
        double L[3] = {Lx, Ly, Lz};

        for (int dim = 0; dim < 3; dim++) {
            gpu_particles[10 * i + dim + 3] += dt * gpu_particles[10 * i + dim + 6] / mass;  // v = v + dt * F/m
            if (gpu_particles[10 * i + dim] < 0) {
                gpu_particles[10 * i + dim] = -gpu_particles[10 * i + dim]; 
                gpu_particles[10 * i + 3 + dim]  = abs(gpu_particles[10 * i + 3 + dim] );
            }
            if (gpu_particles[10 * i + dim] > L[dim]) {
                gpu_particles[10 * i + dim] = 2 * L[dim] - gpu_particles[10 * i + dim];
                gpu_particles[10 * i + 3 + dim] = -abs(gpu_particles[10 * i + 3 + dim]);
            }
        }
    }
}

__global__ 
void computeKineticEnergy(double* gpu_particles, double* gpu_KE, int N) {
    __shared__ double shared_KE[256];  // Shared memory for parallel reduction
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int tid = threadIdx.x;

    double KE = 0.0;
    if (i < N) {
        double vx = gpu_particles[10 * i + 3];
        double vy = gpu_particles[10 * i + 4];
        double vz = gpu_particles[10 * i + 5];
        double mass = gpu_particles[10 * i + 9];

        KE = 0.5 * mass * (vx * vx + vy * vy + vz * vz);
    }
    
    shared_KE[tid] = KE;
    __syncthreads();

    // Parallel reduction in shared memory
    for (int s = blockDim.x / 2; s > 0; s >>= 1) {
        if (tid < s) {
            shared_KE[tid] += shared_KE[tid + s];
        }
        __syncthreads();
    }

    // Write to global memory
    if (tid == 0) {
        atomicAdd(gpu_KE, shared_KE[0]);
    }
}


__global__ 
void scaleVelocities(double* gpu_particles, double lambda, int N) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < N) {
        gpu_particles[10 * i + 3] *= lambda;
        gpu_particles[10 * i + 4] *= lambda;
        gpu_particles[10 * i + 5] *= lambda;
    }
}


/**
 * The simulation is run for T seconds where the particles are updated every timesetep.
 * The kinetic energy is written to a text file every 0.1 seconds.
 * The particle positions in the x-y plane are written to a text file for the test cases.
 * 
 * @param dt The timestep to update the particles
 * @param T The total time to run the simulation
 * @param temp The temperature to be fixed to, however, if equal to -1 not kept constant
 * @param ic_random 'true' if the initial condition is ic_random, 'false' if not, used for writing to text files.
 * @param ic The initial condition being run, used to name the text files appropriately as ic.txt.
 */
void Box::runSimulation(double dt, double T, double temp, bool ic_random, string ic) {
    string particle_file = "Particle_Data/Data/" + ic + ".txt";
    string KE_file = "KE_Data/Data/" + ic + ".txt";
    ofstream particleData(particle_file, ios::out | ios::trunc);
    ofstream KEData(KE_file, ios::out | ios::trunc);

    host_particles = new double [10 * N];  // 3 each for position, velocity, force and 1 for mass
    cudaMalloc(&gpu_particles, 10 * N * sizeof(double));

    for (int i = 0; i < N; i++) {
        host_particles[10 * i]     = particles[i].r[0];
        host_particles[10 * i + 1] = particles[i].r[1];
        host_particles[10 * i + 2] = particles[i].r[2];     // Positions
        
        host_particles[10 * i + 3] = particles[i].get_v()[0];
        host_particles[10 * i + 4] = particles[i].get_v()[1];
        host_particles[10 * i + 5] = particles[i].get_v()[2];   // Velocities
        
        host_particles[10 * i + 6] = particles[i].F[0];
        host_particles[10 * i + 7] = particles[i].F[1];
        host_particles[10 * i + 8] = particles[i].F[2];     // Forces
        
        host_particles[10 * i + 9] = (particles[i].type == 0) ? 1.0 : 10.0; // Mass
    }

    // Copy particle data from host to gpu
    cudaMemcpy(gpu_particles, host_particles, 10 * N * sizeof(double), cudaMemcpyHostToDevice);

    double* gpu_KE;
    cudaMalloc(&gpu_KE, sizeof(double));
    cudaMemset(gpu_KE, 0, sizeof(double));

    int threads = 256;
    int blocks = (N + threads - 1) / threads;

    // Compute initial KE
    computeKineticEnergy<<<blocks, threads>>>(gpu_particles, gpu_KE, N);
    cudaDeviceSynchronize();
    
    double host_KE;
    cudaMemcpy(&host_KE, gpu_KE, sizeof(double), cudaMemcpyDeviceToHost);

    // Scale velocities if temperature is set
    if (temp != -1) {
        double lambda = sqrt((temp * 1.5 * 0.8314459920816467 * double(N)) / host_KE);
        scaleVelocities<<<blocks, threads>>>(gpu_particles, lambda, N);
        cudaDeviceSynchronize();

        computeKineticEnergy<<<blocks, threads>>>(gpu_particles, gpu_KE, N);
        cudaDeviceSynchronize();

        cudaMemcpy(&host_KE, gpu_KE, sizeof(double), cudaMemcpyDeviceToHost);
    }

    KEData << " " << 0.0 << " " << host_KE << endl;

    if (!ic_random) {
        cudaMemcpy(host_particles, gpu_particles, 10 * N * sizeof(double), cudaMemcpyDeviceToHost);
        for (int i = 0; i < N; i++) {
            particleData << " " << 0.0
                        << " " << i + 1
                        << " " << host_particles[10 * i]
                        << " " << host_particles[10 * i + 1]
                        << " " << host_particles[10 * i + 2]
                        << " " << host_particles[10 * i + 3]
                        << " " << host_particles[10 * i + 4]
                        << " " << host_particles[10 * i + 5] << endl;
        }
    }

    for (double t = dt; t < T + dt; t += dt) {
        updatePositions<<<blocks, threads>>>(gpu_particles, dt, N);
        cudaDeviceSynchronize();

        calculateF_i<<<blocks, threads>>>(gpu_particles, N);
        cudaDeviceSynchronize();

        updateVelocities<<<blocks, threads>>>(gpu_particles, dt, N, Lx, Ly, Lz);
        cudaDeviceSynchronize();

        // Compute KE
        cudaMemset(gpu_KE, 0, sizeof(double));
        computeKineticEnergy<<<blocks, threads>>>(gpu_particles, gpu_KE, N);
        cudaDeviceSynchronize();
        cudaMemcpy(&host_KE, gpu_KE, sizeof(double), cudaMemcpyDeviceToHost);

        // Scale velocities if temperature is set
        if (temp != -1) {
            double lambda = sqrt((temp * 1.5 * 0.8314459920816467 * double(N)) / host_KE);
            scaleVelocities<<<blocks, threads>>>(gpu_particles, lambda, N);
            cudaDeviceSynchronize();

            computeKineticEnergy<<<blocks, threads>>>(gpu_particles, gpu_KE, N);
            cudaDeviceSynchronize();

            cudaMemcpy(&host_KE, gpu_KE, sizeof(double), cudaMemcpyDeviceToHost);
        }

        // Write to files every 0.1s
        if (fmod(t, 0.1) < dt) {
            KEData << " " << round(t * 10) / 10 << " " << host_KE << endl;
        }

        if (!ic_random) {
            if (fmod(t, 0.1) < dt) {
                cudaMemcpy(host_particles, gpu_particles, 10 * N * sizeof(double), cudaMemcpyDeviceToHost);
                for (int i = 0; i < N; i++) {
                    particleData << " " << round(t * 10) / 10
                                << " " << i + 1
                                << " " << host_particles[10 * i]
                                << " " << host_particles[10 * i + 1]
                                << " " << host_particles[10 * i + 2]
                                << " " << host_particles[10 * i + 3]
                                << " " << host_particles[10 * i + 4]
                                << " " << host_particles[10 * i + 5] << endl;
                }
            }
        }
    }

    cudaFree(gpu_KE);
    delete[] host_particles;
    cudaFree(gpu_particles);
    particleData.close();
    KEData.close();
}

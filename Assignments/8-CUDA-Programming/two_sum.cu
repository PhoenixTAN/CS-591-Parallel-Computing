#include <iostream>
#include <chrono>       /* time manipulation */

const int LENGTH{1 << 30};
const int MAX_VAL{1 << 4};

/**
 * @description: kernel definition
 * @param {float*} x: vector x
 * @param {float*} y: vector y
 * @param {float*} z: vector z, the addition of x and y
 * @param {int} length: the length of vectors
 */
__global__ void add_vectors(float* x, float* y, float* z, int length) {

    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if ( tid < length ) {
        z[tid] = x[tid] + y[tid];
    }

}

/**
 * @description: initialize vectors
 * @param {float*} x: vector x
 * @param {float*} y: vector y
 * @param {float*} z: vector z
 * @param {float*} z_from_cuda: vector z_from_cuda
 * @param {int} length: the length of vectors
 */
void initialize(float* x, float* y, float* z, float* z_from_cuda, int length) {
    srand(1);
    for ( int i = 0; i < length; i++ ) {
        x[i] = rand() % MAX_VAL;
        y[i] = rand() % MAX_VAL;
        z[i] = 0;
        z_from_cuda[i] = 0;
    }
}

/**
 * @description: sequential version of vector addition
 * @param {float*} x: vector x
 * @param {float*} y: vector y
 * @param {float*} z: vector z
 * @param {int} length: the length of vectors
 */
void sequential_add_vectors(float* x, float* y, float* z, int length) {
    for ( int i = 0; i < length; i++ ) {
        z[i] = x[i] + y[i];
    }
}

/**
 * @description: validate results from sequential version and parallel version
 * @param {float*} h_z: vector h_z
 * @param {float*} h_z_from_cuda: vector h_z_from_cuda
 * @param {int} length: the length of vectors
 */
void validate_result(float* h_z, float* h_z_from_cuda, int length){
    for ( int i = 0; i < length; i++ ) {
        if ( h_z_from_cuda[i] != h_z[i] ){
            std::cout << "False" << std::endl;
            return;
        }
    }
    
    std::cout << "True" << std::endl;
}


int main() {
    
    std::chrono::steady_clock::time_point start_time_cuda;
    std::chrono::steady_clock::time_point end_time_cuda;
    std::chrono::duration<double> duration_cuda;
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
    std::chrono::duration<double> duration;
    double t1;
    double tp;

     // set the ID of the CUDA device
    cudaSetDevice(0);

    // create local data objects
    float* h_x = new float[LENGTH];
    float* h_y = new float[LENGTH];
    float* h_z = new float[LENGTH];
    float* h_z_from_cuda = new float[LENGTH];

    initialize(h_x, h_y, h_z, h_z_from_cuda, LENGTH);

    std::cout << "Running sequential version: " << std::endl;
    start_time = std::chrono::steady_clock::now();

    sequential_add_vectors(h_x, h_y, h_z, LENGTH);

    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time; 
    t1 = duration.count();
    std::cout << "Time: " << t1 << std::endl;
   
    // cuda version
    const int threadsPerBlock = 1024;
    const int numOfBlocks = (LENGTH + threadsPerBlock - 1) / threadsPerBlock;

    size_t size = LENGTH * sizeof(float);

    float* d_x;
    float* d_y;
    float* d_z;

    cudaMalloc(&d_x, size);
    cudaMalloc(&d_y, size);
    cudaMalloc(&d_z, size);

    
    std::cout << "Running cuda version: " << std::endl;
    start_time_cuda = std::chrono::steady_clock::now();

    // Copy vectors from host memory to device memory
    cudaMemcpy(d_x, h_x, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_y, h_y, size, cudaMemcpyHostToDevice);

    // cuda operation
    add_vectors<<<numOfBlocks, threadsPerBlock>>>(d_x, d_y, d_z, LENGTH);

    // Copy result from device memory to host memory
    cudaMemcpy(h_z_from_cuda, d_z, size, cudaMemcpyDeviceToHost);

    end_time_cuda = std::chrono::steady_clock::now();
    duration_cuda = end_time_cuda - start_time_cuda;
    tp = duration_cuda.count();
    std::cout << "Time: " << tp << std::endl;

    // free device memory
    cudaFree(d_x);
    cudaFree(d_y);
    cudaFree(d_z);
    
    // synchronize the GPU preventing premature termination
    cudaDeviceSynchronize();

    validate_result(h_z, h_z_from_cuda, LENGTH);

    // free host memory
    delete[] h_z_from_cuda;
    delete[] h_z;
    delete[] h_x;
    delete[] h_y;

    double speedup = t1 / tp;
    double efficiency = speedup / (numOfBlocks * threadsPerBlock);
    std::cout << "speedup: " << speedup << std::endl;
    std::cout << "efficiency: " << efficiency << std::endl;

    return 0;
}


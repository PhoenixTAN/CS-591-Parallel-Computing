#include <iostream>
#include <chrono>       /* time manipulation */
#include <iomanip>      /* float number precission */

const int LENGTH{1 << 30};
const int MAX_VAL{1 << 5};

/**
 * @description: add up vector a and vector b with CUDA
 * @param {float*} a: matrix A 
 * @param {float*} b: matrix B
 * @param {float*} result: the result of vector addition
 * @param {int} length: the length of the vector
 */
__global__ void add_vectors(float* a, float* b, float* result, int length) {

    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if ( tid < length ) {
        result[tid] = a[tid] + b[tid];
    }

}

/**
 * @description: add up vector a and vector b with CUDA
 * @param {float*} a: matrix A 
 * @param {float*} b: matrix B
 * @param {float*} result: the result of vector addition
 * @param {float*} result_from_cuda: the result of vector addition
 * @param {int} length: the length of the vector
 */
void initialize(float* a, float* b, float* result, float* result_from_cuda, int length) {
    srand(1);
    for ( int i = 0; i < length; i++ ) {
        a[i] = rand() % MAX_VAL / 100.0;
        b[i] = rand() % MAX_VAL / 100.0;
        result[i] = 0;
        result_from_cuda[i] = 0;
    }
}

/**
 * @description: sequential version of adding up vector a and vector b
 * @param {float*} a: matrix A 
 * @param {float*} b: matrix B
 * @param {float*} result: the result of vector addition
 * @param {int} length: the length of the vector
 */
void sequential_add_vectors(float* a, float* b, float* result, int length) {
    for ( int i = 0; i < length; i++ ) {
        result[i] = a[i] + b[i];
    }
}


bool validate_result(float* a, float* b, int length) {
    
    std::cout << "Validating result ... " << std::endl;

    for ( int i = 0; i < length; i++ ) {
        if ( a[i] != b[i] ) {
            std::cout << "false" << std::endl;
            return false;
        }
    }
    
    std::cout << "true" << std::endl;
    return true;
}


int main() {

    // create local data objects
    float* a = new float[LENGTH];
    float* b = new float[LENGTH];
    float* result = new float[LENGTH];
    float* result_from_cuda = new float[LENGTH];

    initialize(a, b, result, result_from_cuda, LENGTH);

    // time manipulation
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
    std::chrono::steady_clock::time_point preprocess_end_time;
    std::chrono::steady_clock::time_point cleanup_end_time;
    std::chrono::duration<double> duration;
    std::chrono::duration<double> preprocess_time;
    std::chrono::duration<double> cleanup_time;

    double t1;
    double tp;
    double t_preprocess;
    double t_cleanup;

    // sequential version
    std::cout << "============================================" << std::endl;
    std::cout << "Running sequential version: " << std::endl;
    start_time = std::chrono::steady_clock::now();
    sequential_add_vectors(a, b, result, LENGTH);
    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time;
    t1 = duration.count();

    std::cout << std::setprecision(2) << "it takes " << t1 << " seconds." << std::fixed << std::scientific << std::endl;
    std::cout << "============================================" << std::endl;

    // cuda version
    const int threadsPerBlock = 256;
    const int blocksPerGrid = (LENGTH + threadsPerBlock - 1) / threadsPerBlock;
    float* cuda_a;
    float* cuda_b;
    float* cuda_result;

    std::cout << "Running cuda version: " << std::endl;
    start_time = std::chrono::steady_clock::now();

    // set the ID of the CUDA device
    cudaSetDevice(0);

    // Allocate vectors in device memory
    cudaMalloc(&cuda_a, sizeof(float) * LENGTH);
    cudaMalloc(&cuda_b, sizeof(float) * LENGTH);
    cudaMalloc(&cuda_result, sizeof(float) * LENGTH);

    // copy vectors from host memory to device memory
    cudaMemcpy(cuda_a, a, sizeof(float) * LENGTH, cudaMemcpyHostToDevice);
    cudaMemcpy(cuda_b, b, sizeof(float) * LENGTH, cudaMemcpyHostToDevice);
    cudaMemcpy(cuda_result, result_from_cuda, sizeof(float) * LENGTH, cudaMemcpyHostToDevice);
    preprocess_end_time = std::chrono::steady_clock::now();
    
    // invoke kernel
    add_vectors<<<blocksPerGrid, threadsPerBlock>>>(cuda_a, cuda_b, cuda_result, LENGTH);
    // copy result from device memory to host memeoy
    cudaMemcpy(result_from_cuda, cuda_result, sizeof(float) * LENGTH, cudaMemcpyDeviceToHost);

    end_time = std::chrono::steady_clock::now();
    duration = end_time - preprocess_end_time;

    // Free device memory
    cudaFree(cuda_a);
    cudaFree(cuda_b);
    cudaFree(cuda_result);
    cleanup_end_time = std::chrono::steady_clock::now();

    // synchronize the GPU preventing premature termination
    cudaDeviceSynchronize();

    tp = duration.count();
    preprocess_time = preprocess_end_time - start_time;
    t_preprocess = preprocess_time.count();
    cleanup_time = cleanup_end_time - end_time;
    t_cleanup = cleanup_time.count();

    std::cout << "it takes " << std::setprecision(2)  << tp << " seconds." << std::scientific << std::endl;
    std::cout << "preprocess time: " << std::setprecision(2)  << t_preprocess  << std::scientific << " seconds." << std::endl;
    std::cout << "cleanup_time: " << std::setprecision(2)  << t_cleanup << std::scientific << " seconds." << std::endl;
    std::cout << "total: " << std::setprecision(2)  << tp + t_preprocess + t_cleanup << std::scientific << " seconds." << std::endl;
    std::cout << "============================================" << std::endl;

    // validating result
    validate_result(result, result_from_cuda, LENGTH);

    // cleanup host memory
    delete[] a;
    delete[] b;
    delete[] result;
    delete[] result_from_cuda;

    std::cout << "============================================" << std::endl;

    // calculate speed up
    double speedup = t1 / tp;
    std::cout << "speedup: " << speedup << std::scientific << std::endl;

    // efficiency
    double efficiency = speedup / blocksPerGrid / threadsPerBlock;
    std::cout << "efficiency: " << efficiency << std::scientific << std::endl;

    std::cout << "============================================" << std::endl;
    std::cout << "if we take the preprocess time and the cleanup time as a cost of cuda computation" << std::endl;
    speedup = t1 / (tp + t_preprocess + t_cleanup);
    std::cout << "speedup: " << speedup << std::scientific << std::endl;
    efficiency = speedup / blocksPerGrid / threadsPerBlock;
    std::cout << "efficiency: " << efficiency << std::scientific << std::endl;
    std::cout << "============================================" << std::endl;

    return 0;
}


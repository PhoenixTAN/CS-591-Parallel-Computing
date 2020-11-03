#include <iostream>
#include <chrono>       /* time manipulation */
#include <omp.h>        /* openMP */

/**
 * @description: multiply A * B in the ordinary fashion
 * @param {long int*} A: matrix A 
 * @param {long int*} B: matrix B
 * @param {int} matrix_size: the size of the matrix
 * @return {long int*} the result of A * B 
 */
void sequential_matrix_multiplication(long int* A, long int* B, long int* C, int matrix_size) {
    
    // perform matrix multiplication
    for ( int i = 0; i < matrix_size; i++ ) {
        for ( int j = 0; j < matrix_size; j++ ) {
            long int temp_sum = 0;
            for ( int k = 0; k < matrix_size; k++ ) {
                temp_sum += A[matrix_size * i + k] * B[matrix_size * k + j];
            }
            C[matrix_size * i + j] = temp_sum;
        }
    } 
}


/**
 * @description: multiply A * B with openMP API
 * @param {long int*} A: matrix A 
 * @param {long int*} B: matrix B
 * @param {int} matrix_size: the size of the matrix
 * @return {long int*} the result of A * B 
 */
void openMP_matrix_multiplication(long int* A, long int* B, long int* C, int matrix_size) {
    
    // maximum number of threads
    const int max_num_of_threads = omp_get_max_threads();
    std::cout<< "number of threads: " << max_num_of_threads << std::endl;

    int i = 0;
    int j = 0;
    int k = 0;
    long int temp_sum = 0;

    // perform matrix multiplication
    #pragma omp parallel for private(j) private(k) private(temp_sum) num_threads(max_num_of_threads)
    for ( i = 0; i < matrix_size; i++ ) {
        for ( j = 0; j < matrix_size; j++ ) {
            temp_sum = 0;
            #pragma omp parallel for reduction(+:temp_sum)
            for ( k = 0; k < matrix_size; k++ ) {
                temp_sum += A[matrix_size * i + k] * B[matrix_size * k + j];
            }
            C[matrix_size * i + j] = temp_sum;
        }
    } 
}


/**
 * @description: validate the result from two matrix multiplication method
 * @param {long int*} A: matrix A
 * @param {long int*} B: matrix B
 * @param {int} matrix_size: the size of matrix
 * @return {bool} if the result is correct, return true 
 */
bool validate_result(long int* A, long int* B, int matrix_size) {
    
    std::cout << "validating results: ";

    for ( int i = 0; i < matrix_size * matrix_size; i++ ) {
        if ( A[i] != B[i] ) {
            std::cout << "false" << std::endl;
            return false;
        }
    }

    std::cout << "true" << std::endl;
    return true;
}


int main() {

    const int matrix_size = 1 << 10;
    long int N = matrix_size * matrix_size;

    // initialization
    long int* A = new long int[N];
    long int* B = new long int[N];
    long int* C = new long int[N];
    long int* C_openMP = new long int[N];

    // initialize matrix A, B and C
    for ( int i = 0; i < N; i++ ) {
        A[i] = i + 1;
        B[i] = N - i;
        C[i] = 0;
        C_openMP[i] = 0;
    }

    // time manipulation
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
    std::chrono::duration<double> duration;

    // sequential version
    start_time = std::chrono::steady_clock::now();
    std::cout << "Running sequential version: " << std::endl;
    sequential_matrix_multiplication(A, B, C, matrix_size);
    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time;
    std::cout << "it takes " << duration.count() << " seconds." << std::endl;
    std::cout << "============================================" << std::endl;
    
    // openMP version
    start_time = std::chrono::steady_clock::now();
    std::cout << "Running openMP version: " << std::endl;
    openMP_matrix_multiplication(A, B, C_openMP, matrix_size);
    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time; 
    std::cout << "it takes " << duration.count() << " seconds." << std::endl;
    std::cout << "============================================" << std::endl;

    // validate result
    validate_result(C, C_openMP, matrix_size);

    return 0;
}

#include <iostream>
#include <chrono>       /* time manipulation */
#include <omp.h>        /* openMP */
#include <stdlib.h>     /* random number */


/**
 * @description: sequential version of repetitive smoothing of a vector
 * @param {int* v} pre-initialized array of length N
 * @param {int* s} the smoothed version of v, preinitialized with v
 * @param {int M} the number of iterations
*/
void sequential_vector_repetitive_smoothing(int* v, int* s, int N, int M) {
    for (int i = 0; i < M; i++){
        for (int j = 2; j < N - 2; j++) {
            s[j] = 0;
            for (int k = -2; k < 3; k++) {
                s[j] += 0.2 * v[j + k];
            }
        }
        for (int j = 0; j < N; j++) {
            v[j] = s[j];
        }
    }
}

/**
 * @description: openMP version of repetitive smoothing of a vector
 * @param {int* v} pre-initialized array of length N
 * @param {int* s} the smoothed version of v, preinitialized with v
 * @param {int M} the number of iterations
*/
void openMP_vector_repetitive_smoothing(int* v, int* s, int N, int M) {

    const int max_num_of_threads = omp_get_max_threads();
    std::cout<< "number of threads: " << max_num_of_threads << std::endl;

    for (int i = 0; i < M; i++) {

        int k = -2;
        #pragma omp parallel for private(k) num_threads(max_num_of_threads)
        for (int j = 2; j < N - 2; j++) {
            s[j] = 0;
            for (k = -2; k < 3; k++) {
                s[j] += 0.2 * v[j + k];
            }
        }

        #pragma omp parallel for num_threads(max_num_of_threads)
        for (int j = 0; j < N; j++) {
            v[j] = s[j];
        }
    }
}

/**
 * @description: validate the result from two matrix multiplication method
 * @param {int*} A: matrix A
 * @param {int*} B: matrix B
 * @param {int} size: the size of array
 * @return {bool} if the result is correct, return true 
 */
bool validate_result(int* A, int* B, int size) {
    
    std::cout << "validating results: ";

    for ( int i = 0; i < size; i++ ) {
        if ( A[i] != B[i] ) {
            std::cout << "false" << std::endl;
            return false;
        }
    }

    std::cout << "true" << std::endl;
    return true;
}


int main() {

    const int N = 1 << 20;
    const int M = 5;

    // initialization
    int* v = new int[N];
    int* s = new int[N];
    int* s_openMP = new int[N];
    int* v_openMP = new int[N];

    // initialize matrix w and v
    // random number generator
    srand(time(NULL));                    
    for ( int i = 0; i < N; i++ ) {
        v[i] = rand();
        s[i] = v[i];
        v_openMP[i] = v[i];
        s_openMP[i] = v[i];
    }

    // time manipulation
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
    std::chrono::duration<double> duration;

    // sequential version
    start_time = std::chrono::steady_clock::now();
    std::cout << "Running sequential version: " << std::endl;
    sequential_vector_repetitive_smoothing(v, s, N, M);
    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time;
    std::cout << "it takes " << duration.count() << " seconds." << std::endl;
    std::cout << "============================================" << std::endl;
    
    // openMP version
    start_time = std::chrono::steady_clock::now();
    std::cout << "Running openMP version: " << std::endl;
    openMP_vector_repetitive_smoothing(v_openMP, s_openMP, N, M);
    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time; 
    std::cout << "it takes " << duration.count() << " seconds." << std::endl;
    std::cout << "============================================" << std::endl;

    // validate result
    validate_result(s, s_openMP, N);

    return 0;
}

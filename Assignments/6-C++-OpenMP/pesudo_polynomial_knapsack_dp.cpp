#include <iostream>
#include <chrono>       /* time manipulation */
#include <omp.h>        /* openMP */

#define AT(i, j)    ( (i) * (C+1) + (j) )
#define MAX(x, y)   ( (x) < (y) ? (y) : (x) )

/**
 * @description: sequential version of pseudo polynomial knapsack
 * @param {int* w} a constant and non-negative array of length N
 * @param {int* v} a constant and non-negative array of length N
 * @param {int* m} an array of length (C+1)*(N+1) initialized with zeros
*/
void sequential_pseudo_polynomial_knapsack(int* w, int* v, int* m, int C, int N) {

    for ( int i = 1; i < N + 1; i++ ) {
        for ( int j = 0; j < C + 1; j++ ) {
            if ( w[i-1] <= j ) {
                m[AT(i, j)] = MAX(m[AT(i-1, j)], m[AT(i-1, j - w[i-1])] + v[i-1]);
            }
            else {
                m[AT(i,j)] = m[AT(i-1, j)];
            }
        }
    }
}

/**
 * @description: openMP version of pseudo polynomial knapsack
 * @param {int* w} a constant and non-negative array of length N
 * @param {int* v} a constant and non-negative array of length N
 * @param {int* m} an array of length (C+1)*(N+1) initialized with zeros
*/
void openMP_pseudo_polynomial_knapsack(int* w, int* v, int* m, int C, int N) {

    // maximum number of threads
    const int max_num_of_threads = omp_get_max_threads();
    std::cout<< "number of threads: " << max_num_of_threads << std::endl;

    for ( int i = 1; i < N + 1; i++ ) {
        int j = 0;
        #pragma omp parallel for private(j) num_threads(max_num_of_threads)
        for ( j = 0; j < C + 1; j++ ) {
            if ( w[i-1] <= j ) {
                m[AT(i, j)] = MAX(m[AT(i-1, j)], m[AT(i-1, j-w[i-1])] + v[i-1]);
            }
            else {
                m[AT(i,j)] = m[AT(i-1, j)];
            }
        }
    }
    
}

/**
 * @description: validate the result from two matrix multiplication method
 * @param {int*} A: matrix A
 * @param {int*} B: matrix B
 * @param {int} matrix_size: the size of matrix
 * @return {bool} if the result is correct, return true 
 */
bool validate_result(int* A, int* B, int matrix_size) {
    
    std::cout << "validating results: ";

    for ( int i = 0; i < matrix_size; i++ ) {
        if ( A[i] != B[i] ) {
            std::cout << "false" << std::endl;
            return false;
        }
    }

    std::cout << "true" << std::endl;
    return true;
}


int main() {

    const int N = 1 << 15;
    const int C = 1 << 13;

    // initialization
    int* w = new int[N];
    int* v = new int[N];
    int* m = new int[(C+1)*(N+1)];
    int* m_openMP = new int[(C+1)*(N+1)];

    // initialize matrix w and v
    srand(time(NULL));  
    for ( int i = 0; i < N; i++ ) {
        w[i] = rand();
        v[i] = rand();
    }

    // initialize matrix m and m_openMP
    for ( int i = 0; i < (C+1)*(N+1); i++ ) {
        m[i] = 0;
        m_openMP[i] = 0;
    }

    // time manipulation
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
    std::chrono::duration<double> duration;

    // sequential version
    start_time = std::chrono::steady_clock::now();
    std::cout << "Running sequential version: " << std::endl;
    sequential_pseudo_polynomial_knapsack(w, v, m, C, N);
    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time;
    std::cout << "it takes " << duration.count() << " seconds." << std::endl;
    std::cout << "============================================" << std::endl;
    
    // openMP version
    start_time = std::chrono::steady_clock::now();
    std::cout << "Running openMP version: " << std::endl;
    openMP_pseudo_polynomial_knapsack(w, v, m_openMP, C, N);
    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time; 
    std::cout << "it takes " << duration.count() << " seconds." << std::endl;
    std::cout << "============================================" << std::endl;

    // validate result
    validate_result(m, m_openMP, (C+1)*(N+1));

    return 0;
}

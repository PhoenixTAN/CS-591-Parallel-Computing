/*
 * @Author: Ziqi Tan, Xueyan Xia
 * @Description: Assignment 4: C++ Thread
 */

#include <iostream>
#include <chrono>       /* time manipulation */
#include <string>     

#include "utils.cpp"                        /* functions to transpose matrix, print matrix and validate results */
#include "sequential.cpp"                   /* Sequential execution of the standard algorithm (without threads). */
#include "sequential_cache_locality.cpp"    /* Sequential execution of an implementation using the transpose of the second matrix to improve cache locality. */
#include "multithreading.cpp"               /* Parallel execution with normal matrices, using 8 threads and 8 tiles. */
#include "multithreading_cache_locality.cpp"/* Parallel execution (as above), but using the transpose of the second matrix.*/


#define MATRIX_SIZE 1024
#define N MATRIX_SIZE * MATRIX_SIZE
#define NUM_OF_THREADS 8

static long int* A;         /* matrix A */
static long int* B;         /* matrix B */
static long int* BT;        /* B transpose */
static long int* C;         /* result from multiplying A * B in the ordinary fashion */
static long int* C_plus;    /* result from other methods */


int main ( int argc, char *argv[] ) {

    if ( argc != 2 ) {
        std::cout << "Please input \"sequential_plus\", \"parallel\", \"parallel_plus\"." << std::endl;
        return 0;
    }

    // initialization
    A = new long int[N];
    B = new long int[N];
    BT = NULL;   
    C = NULL;
    C_plus = NULL;    
    
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
    std::chrono::duration<double> duration;

    // initialize matrix A and matrix B
    for ( int i = 0; i < N; i++ ) {
        A[i] = i + 1;
        B[i] = N - i;
    }

    // get the transpose of matrix B
    BT = transpose(B, MATRIX_SIZE);

    // multiply A * B in the ordinary fashion
    start_time = std::chrono::steady_clock::now();
    C = sequential_matrix_multiplication(A, B, MATRIX_SIZE);
    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time;
    std::cout << "ordinary fashion takes " << duration.count() << " seconds." << std::endl;

    // multiply A * Bt (B-transpose), after transposing matrix
    start_time = std::chrono::steady_clock::now();
    
    const std::string method = std::string(argv[1]);
    if ( method == "sequential_plus" ) {
        C_plus = sequential_matrix_multiplication_plus(A, BT, MATRIX_SIZE);
    }
    else if ( method == "parallel" ) {
        C_plus = parallel_matrix_multiplication(A, B, MATRIX_SIZE, NUM_OF_THREADS);
    }
    else if ( method == "parallel_plus" ) {
        C_plus = parallel_matrix_multiplication_plus(A, BT, MATRIX_SIZE, NUM_OF_THREADS);
    }
    else {
        std::cout << "Please input \"sequential_plus\", \"parallel\", \"parallel_plus\"."<< std::endl; 
    }

    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time;

    std::cout << "new method takes " << duration.count() << " seconds."  << std::endl;

    validate_result(C, C_plus, MATRIX_SIZE, 1);

    // remember to free memory
    delete[] A;
    delete[] B;
    delete[] BT;
    delete[] C;
    delete[] C_plus;

    return 0;
}

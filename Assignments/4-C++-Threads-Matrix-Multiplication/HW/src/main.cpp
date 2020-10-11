#include <iostream>
#include <chrono>                       /* time manipulation */
#include "utils.cpp"
#include "sequential.cpp"
#include "sequential_cache_locality.cpp"

#define ROWS 1024
#define COLUMNS 1024

static long int* A;         /* matrix A */
static long int* B;         /* matrix B */
static long int* BT;        /* B transpose */
static long int* C;         /* result from multiplying A * B in the ordinary fashion */
static long int* C_plus;    /* result from other methods */



int main ( int argc, char *argv[] ) {

    std::cout << argc << std::endl;

    for ( int i = 0; i < argc; i++ ) {
        std::cout << argv[i] << std::endl;
    }

    // initialization
    A = new long int[ROWS * COLUMNS];
    B = new long int[ROWS * COLUMNS];
    BT = NULL;   
    C = NULL;
    C_plus = NULL;    
    
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
    std::chrono::duration<double> duration;

    // initialize matrix A and matrix B
    for ( int i = 0; i < ROWS * COLUMNS; i++ ) {
        A[i] = i + 1;
        B[i] = ROWS * COLUMNS - i;
    }

    // get the transpose of matrix B
    BT = transpose(B, ROWS, COLUMNS);

    // multiply A * B in the ordinary fashion
    start_time = std::chrono::steady_clock::now();
    C = sequential_matrix_multiplication(A, B, ROWS, COLUMNS, COLUMNS);
    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time;
    std::cout << "ordinary fashion takes " << duration.count() << " seconds." << std::endl;

    // multiply A * Bt (B-transpose), after transposing matrix
    start_time = std::chrono::steady_clock::now();
    C_plus = sequential_matrix_multiplication_plus(A, BT, ROWS, COLUMNS, COLUMNS);
    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time;
    std::cout << "new method takes " << duration.count() << " seconds."  << std::endl;

    validate_result(C, C_plus, ROWS, COLUMNS, 1);

    // remember to free memory
    delete[] A;
    delete[] B;
    delete[] BT;
    delete[] C;
    delete[] C_plus;

    return 0;
}

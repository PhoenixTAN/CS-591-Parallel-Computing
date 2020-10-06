/*
 * @Author: Ziqi Tan, Xueyan Xia
 * @Date: 2020-10-04 13:32:19
 * @LastEditTime: 2020-10-05 13:02:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \CS-591-Parallel-Computing\Assignments\3-Cache-Locality\CacheLocality\cacheLocality.cpp
 */

/* head files */
#include <iostream>
#include <chrono>   /* time manipulation */

using std::cout;
using std::endl;

/* functions */
long int* transpose(long int* M, int rows, int cols);
void print_matrix(long int* M, int rows, int cols);
long int* matrix_multiplication(long int* A, long int* B, int rows, int cols, int M);
long int* matrix_multiplication_plus(long int* A, long int* B, int rows, int cols, int M);


/**
 * @description: transpose a matrix M and MT is the result
 * @param {long int*} m: the matrix you need to transpose
 * @param {int} rows: the number of rows
 * @param {int} cols: the number of cols
 * @return {long int*} the matrix after transpose
 */
long int* transpose(long int* M, int rows, int cols) {
    long int* MT = new long int[rows*cols];
    for ( int i = 0; i < rows; i++ ) {
        for ( int j = 0; j < cols; j++ ) {
            MT[j*rows + i] = M[i*rows + j];
        }
    }
    return MT;
}


/**
 * @description: print a matrix
 * @param {long int*} M: the matrix you want to print
 * @param {int} rows: the number of rows
 * @param {int} cols: the number of cols
 */
void print_matrix(long int* M, int rows, int cols) {
    for ( int i = 0; i < rows; i++ ) {
        for ( int j = 0; j < cols; j++ ) {
            cout << M[rows*i + j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

/**
 * @description: multiply A * B in the ordinary fashion
 * @param {long int*} A: matrix A 
 * @param {long int*} B: matrix B
 * @param {int} rows: the number of rows of the result matrix
 * @param {int} cols: the number of columns of the result matrix
 * @param {int} M: the number of columns of A, also the number of rows of B
 * @return {long int*} the result of A * B 
 */
long int* matrix_multiplication(long int* A, long int* B, int rows, int cols, int M) {
    long int* C = new long int[rows*cols];
    
    // initialize matrix C
    for ( int i = 0; i < rows*cols; i++ ) {
        C[i] = 0;
    }

    // perform matrix multiplication
    for ( int i = 0; i < rows; i++ ) {
        for ( int j = 0; j < cols; j++ ) {
            for ( int k = 0; k < M; k++ ) {
                C[rows * i + j] += A[rows * i + k] * B[rows * k + j];
            }
        }
    }
    return C;
}


/**
 * @description: multiply A * Bt (B-transpose), after transposing matrix
 * @param {long int*} A: matrix A 
 * @param {long int*} Bt: matrix Bt
 * @param {int} rows: the number of rows of the result matrix
 * @param {int} cols: the number of columns of the result matrix
 * @param {int} M: the number of columns of A, also the number of rows of Bt
 * @return {long int*} the result of A * B 
 */
long int* matrix_multiplication_plus(long int* A, long int* B, int rows, int cols, int M) {

    long int* C = new long int[rows*cols];

    // initialize matrix C
    for ( int i = 0; i < rows*cols; i++ ) {
        C[i] = 0;
    }

    for ( int i = 0; i < rows; i++ ) {
        for ( int j = 0; j < cols; j++ ) {
            for ( int k = 0; k < M; k++ ) {
                C[rows * i + j] += A[rows * i + k] * B[rows * j + k];
            }
        }
    }

    return C;
}

/**
 * @description: validate the result from two matrix multiplication method
 * @param {long int*} A: matrix A
 * @param {long int*} B: matrix B
 * @param {long int*} rows: the number of rows
 * @param {long int*} cols: the number of columns 
 * @return {bool} if the result is correct, return true 
 */
bool validate_result(long int* A, long int* B, int rows, int cols) {
    
    for ( int i = 0; i < rows * cols; i++ ) {
        if ( A[i] != B[i] ) {
            return false;
        }
    }

    return true;
}


int main() {

    
    cout << "Running ..." << endl;
    cout << "It takes 20 seconds or so ..." << endl;

    // we assume all matrix is square matrix
    const int rows = 1024;
    const int cols = 1024;

    // declare
    long int* A = new long int[rows * cols];
    long int* B = new long int[rows * cols];
    
    // B transpose
    long int* BT = NULL;

    // result from multiplying A * B in the ordinary fashion      
    long int* C = NULL;

    // result from multiplying A * Bt (B-transpose), after transposing matrix     
    long int* C_plus = NULL;    
    
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
    std::chrono::duration<double> duration;

    // initialize matrix A and matrix B
    for ( int i = 0; i < rows * cols; i++ ) {
        A[i] = i + 1;
        B[i] = rows * cols - i;
    }

    // get the transpose of matrix B
    BT = transpose(B, rows, cols);

    // multiply A * B in the ordinary fashion
    start_time = std::chrono::steady_clock::now();
    C = matrix_multiplication(A, B, rows, cols, cols);
    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time;
    cout << "ordinary fashion takes " << duration.count() << " seconds." << endl;

    // multiply A * Bt (B-transpose), after transposing matrix
    start_time = std::chrono::steady_clock::now();
    C_plus = matrix_multiplication_plus(A, BT, rows, cols, cols);
    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time;
    cout << "new method takes " << duration.count() << " seconds."  << endl;

    cout << "validating results: ";
    bool flag = validate_result(C, C_plus, rows, cols);
    if ( flag ) {
        cout << "true" << endl;
    }
    else {
        cout << "false" << endl;
    }

    // remember to free memory
    delete[] A;
    delete[] B;
    delete[] BT;
    delete[] C;
    delete[] C_plus;
}


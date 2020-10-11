#include<iostream>
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
            std::cout << M[rows*i + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


/**
 * @description: validate the result from two matrix multiplication method
 * @param {long int*} A: matrix A
 * @param {long int*} B: matrix B
 * @param {long int*} rows: the number of rows
 * @param {long int*} cols: the number of columns 
 * @param {int} verbose: 0 to print nothing, 1 to print validate result
 * @return {bool} if the result is correct, return true 
 */
bool validate_result(long int* A, long int* B, int rows, int cols, int verbose) {
    
    std::cout << "validating results: ";

    for ( int i = 0; i < rows * cols; i++ ) {
        if ( A[i] != B[i] ) {
            std::cout << "false" << std::endl;
            return false;
        }
    }

    std::cout << "true" << std::endl;
    return true;
}

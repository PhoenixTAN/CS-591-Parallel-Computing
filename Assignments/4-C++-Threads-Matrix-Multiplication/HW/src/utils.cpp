/*
 * @Author: Ziqi Tan, Xueyan Xia
 * @Description: functions to transpose matrix, print matrix and validate results
 */

#include<iostream>

long int* transpose(long int* M, int matrix_size);
void print_matrix(long int* M, int matrix_size);
bool validate_result(long int* A, long int* B, int matrix_size, int verbose);

/**
 * @description: transpose a matrix M and MT is the result
 * @param {long int*} m: the matrix you need to transpose
 * @param {int} matrix_size: the size of matrix
 * @return {long int*} the matrix after transpose
 */
long int* transpose(long int* M, int matrix_size) {
    long int* MT = new long int[matrix_size * matrix_size];
    for ( int i = 0; i < matrix_size; i++ ) {
        for ( int j = 0; j < matrix_size; j++ ) {
            MT[j*matrix_size + i] = M[i*matrix_size + j];
        }
    }
    return MT;
}


/**
 * @description: print a matrix
 * @param {long int*} M: the matrix you want to print
 * @param {int} matrix_size: the size of matrix
 */
void print_matrix(long int* M, int matrix_size) {
    for ( int i = 0; i < matrix_size; i++ ) {
        for ( int j = 0; j < matrix_size; j++ ) {
            std::cout << M[matrix_size * i + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


/**
 * @description: validate the result from two matrix multiplication method
 * @param {long int*} A: matrix A
 * @param {long int*} B: matrix B
 * @param {int} matrix_size: the size of matrix
 * @param {int} verbose: 0 to print nothing, 1 to print validate result
 * @return {bool} if the result is correct, return true 
 */
bool validate_result(long int* A, long int* B, int matrix_size, int verbose) {
    
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

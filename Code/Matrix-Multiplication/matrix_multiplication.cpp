//
//  matrix_multiplication.cpp
//  CacheLocality
//
//  Created by Amittai Aviram on 10/9/20.
//  Copyright Â© 2020 Amittai Aviram. All rights reserved.
//

/**
 @file matrix_multiplication.cpp
 Supports a comparison between matrix multiplication of A*B using a normal matrix B
 with the same multiplication using the transpose of B.
 @author Amittai Aviram amittai@bu.edu
 @date 2020-10-10
 */

#include <array>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>

/**
 Log2 of the dimension of all matrices in this program.
 */
const uint32_t LOG_DIMENSION = 10;

/**
 Dimension of all matrices in this program, which are square.
 */
const uint32_t DIMENSION = 1 << LOG_DIMENSION;

/**
 Number of entries in each matrix in this program.
 */
const uint32_t NUM_ENTRIES = DIMENSION * DIMENSION;

/**
 Type representing a matrix, to be indexed in column-major format as i * DIMENSION + j.
 */
typedef std::array<int64_t, NUM_ENTRIES> matrix_t;

/**
 Type representing a smart pointer to a matrix.  Dereference and use the "at" function to index.
 */
typedef std::unique_ptr<matrix_t> matrrix_ptr_t;


/**
 Multiplies the two input matrices using the standard algorithm,
 placing the results in the matrix passed in as the third parameter.
 @param[in] matrix_a The first input matrix operand.
 @param[in] matrix_b The second matrix operand.
 @param[out] matrix_c The result, passed in as a zero-initialized matrix.
 */
void multiply_standard(
                       matrrix_ptr_t& matrix_a,
                       matrrix_ptr_t& matrix_b,
                       matrrix_ptr_t& matrix_c
                       ) {
    for (uint32_t i = 0; i < DIMENSION; ++i) {
        for (uint32_t j = 0; j < DIMENSION; ++j) {
            for (uint32_t k = 0; k < DIMENSION; ++k) {
                matrix_c->at(i * DIMENSION + j) += matrix_a->at(i * DIMENSION + k) *
                    matrix_b->at(k * DIMENSION + j);
            }
        }
    }
}


/**
 Multiplies two input matrices, where the second input matrix is the transpose of the original operand.
 @param[in] matrix_a The first input matrix.
 @param[in] matrix_bt  The transpose of the second input matrix.
 @param[out] matrix_c The result, passed in as a zero-initialized matrix.
 */
void multiply_transpose(
                       matrrix_ptr_t& matrix_a,
                       matrrix_ptr_t& matrix_bt,
                       matrrix_ptr_t& matrix_c
                       ) {
    for (uint32_t i = 0; i < DIMENSION; ++i) {
        for (uint32_t j = 0; j < DIMENSION; ++j) {
            for (uint32_t k = 0; k < DIMENSION; ++k) {
                matrix_c->at(i * DIMENSION + j) += matrix_a->at(i * DIMENSION + k) *
                    matrix_bt->at(j * DIMENSION + k);
            }
        }
    }
}

/**
 Checks two matrices for equality.
 @param[in] matrix_a The first input matrix.
 @param[in] matrix_b The second input matrix.
 @return True if the two input matrices contain the same values under the same indices, false otherwise.
 */
bool are_equal(matrrix_ptr_t& matrix_a, matrrix_ptr_t& matrix_b) {
    for (uint32_t i = 0; i < DIMENSION; ++i) {
        for (uint32_t j = 0; j < DIMENSION; ++j) {
            if (matrix_a->at(i * DIMENSION + j) != matrix_b->at(i * DIMENSION + j)) {
                return false;
            }
        }
    }
    return true;
}


/**
 Writes the transpose of the input matrix to an output matrix object.
 @param[in] matrix The input matrix.
 @param[out] transpose AThe transpose of the input matrix as output.
 */
void create_transpose_matrix(matrrix_ptr_t& matrix, matrrix_ptr_t& transpose) {
    for (uint32_t i = 0; i < DIMENSION; ++i) {
        for (uint32_t j = 0; j < DIMENSION; ++j) {
            transpose->at(j * DIMENSION + i) = matrix->at(i * DIMENSION + j);
        }
    }
}


/**
 Prints the input matrix in a readable form (if the matrix is small enough).
 @param[in] matrix The input matrix to be printed.
 */
void print_matrix(matrrix_ptr_t& matrix) {
    std::cout << "-----------------" << std::endl;
    for (uint32_t i = 0; i < DIMENSION; ++i) {
        for (uint32_t j = 0; j < DIMENSION; ++j) {
            std::cout << matrix->at(i * DIMENSION + j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-----------------" << std::endl;
}


/**
 - Creates and initialize two square matrices, A and B.
 - Creates the transpose of matrix B.
 - Runs the matrix multiplication A * B in two ways --
  - Using B
  - Using the transpose of B
 - Checks the two result matrices for equality.
 - Prints out the results of the equality check.
 - Prints out the timing results of the two respective multiplication algorithms.
 */
int main(void) {
    std::cout << "Comparison of matrix multiplication A * B\n";
    std::cout << "using normal B and its transpose (Bt), respectively." << std::endl;

    matrrix_ptr_t matrix_a(new matrix_t());
    matrrix_ptr_t matrix_b(new matrix_t());
    matrrix_ptr_t matrix_bt(new matrix_t());
    matrrix_ptr_t matrix_c(new matrix_t());
    matrrix_ptr_t matrix_c_from_transpose(new matrix_t());

    for (uint32_t i = 0, j = NUM_ENTRIES; i < NUM_ENTRIES; ++i, --j) {
        matrix_a->at(i) = i;
        matrix_b->at(i) = j;
    }
    create_transpose_matrix(matrix_b, matrix_bt);

    auto start = std::chrono::high_resolution_clock::now();
    multiply_standard(matrix_a, matrix_b, matrix_c);
    auto stop = std::chrono::high_resolution_clock::now();
    auto standard_time = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    start = std::chrono::high_resolution_clock::now();
    multiply_transpose(matrix_a, matrix_bt, matrix_c_from_transpose);
    stop = std::chrono::high_resolution_clock::now();
    auto transpose_time = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    bool correct = are_equal(matrix_c, matrix_c_from_transpose);
    std::cout << "The results of the two multiplication algorithms are " <<
        (correct ? "" : "not ") << "equal." << std::endl;
    std::cout << "Time - standard algorithm: " << standard_time << std::endl;
    std::cout << "Time - using transpose: " << transpose_time << std::endl;
    double speedup = static_cast<double>(standard_time)/static_cast<double>(transpose_time);
    std::cout << "Speedup: " << speedup << std::endl;

    return 0;
}
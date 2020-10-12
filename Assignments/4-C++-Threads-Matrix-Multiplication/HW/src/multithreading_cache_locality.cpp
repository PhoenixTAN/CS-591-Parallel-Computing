/*
 * @Author: Ziqi Tan, Xueyan Xia
 * @Description: Parallel execution, but using the transpose of the second matrix.
 */

#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>    // std::max, std::min

std::mutex mutex_plus;

long int* parallel_matrix_multiplication_plus(long int* A, long int* B, 
                                            int matrix_size, 
                                            int num_of_threads);
void worker_plus(int id, int start, int matrix_size,
                long int* A, long int* B, long int* C,
                int num_of_threads);


/**
 * @description: multiply A * B in the ordinary fashion
 * @param {long int*} A: matrix A 
 * @param {long int*} B: matrix B
 * @param {int} matrix_size: the size of matrix
 * @param {int} num_of_threads: the number of threads
 * @return {long int*} the result of A * B 
 */
long int* parallel_matrix_multiplication_plus(long int* A, long int* B, 
                                            int matrix_size, 
                                            int num_of_threads) {
    
    int N = matrix_size * matrix_size;

    long int* C = new long int[N];

    // initialize matrix C
    for ( int i = 0; i < N; i++ ) {
        C[i] = 0;
    }

    std::vector<std::thread> threads;
    std::mutex mutex;

    int interval = N / num_of_threads;
    int index = 0;      // start index for the worker
    int thread_id = 0;

    // initialize threads
    while ( index < N ) {
        threads.emplace_back(std::thread(worker_plus, thread_id, index, matrix_size, A, B, C, num_of_threads));
        index += interval;
        thread_id++;
    }

    // join threads
    for ( std::thread& thread : threads ) {
        thread.join();
    }

    return C;
}


/**
 * @description: multiply A * B in the ordinary fashion
 * @param {long int*} A: matrix A 
 * @param {long int*} B: matrix B
 * @param {int} matrix_size: the size of matrix
 * @param {int} num_of_threads: the number of threads
 * @return {long int*} the result of A * B 
 */
void worker_plus(int id, int start, int matrix_size,
                long int* A, long int* B, long int* C,
                int num_of_threads
                ) {
    
    int N = matrix_size * matrix_size;

    // calculate the size of the temporary result
    int num_of_rows = std::max(1, matrix_size / num_of_threads);
    int num_of_columns = matrix_size;

    // initialize a temporary result matrix C
    long int* temp_C = new long int[num_of_rows * num_of_columns];
    for ( int i = 0; i < num_of_rows * num_of_columns; i++ ) {
        temp_C[i] = 0;
    }

    // used for the inner iteration
    int chunk_size = std::min(matrix_size, N / num_of_threads);

    // i and j iterate the temporary matrix C
    for ( int i = 0; i < num_of_rows; i++ ) {
        for ( int j = 0; j < num_of_columns; j++ ) {
            for ( int k = 0; k < chunk_size; k++ ) {
                int index_of_A = i * matrix_size + k + start;
                int index_of_B = (j + start % matrix_size) * matrix_size + k;
                int index_of_temp_C = i * matrix_size + j;
                temp_C[index_of_temp_C] += A[index_of_A] * B[index_of_B];
            }
        }
    }

    // actual row start index in C
    int start_row_of_C = start / matrix_size;

    // matrix C should be locked here
    const std::lock_guard<std::mutex> lock(mutex_plus);
    // put temp_C into C
    for ( int i = 0; i < num_of_rows * num_of_columns; i++ ) {
        C[i + start_row_of_C * matrix_size] += temp_C[i];
    }

}

/**
 * @description: multiply A * B in the ordinary fashion
 * @param {long int*} A: matrix A 
 * @param {long int*} B: matrix B
 * @param {int} matrix_size: the size of the matrix
 * @return {long int*} the result of A * B 
 */
long int* sequential_matrix_multiplication(long int* A, long int* B, int matrix_size) {
    long int* C = new long int[matrix_size * matrix_size];
    
    // initialize matrix C
    for ( int i = 0; i < matrix_size * matrix_size; i++ ) {
        C[i] = 0;
    }

    // perform matrix multiplication
    for ( int i = 0; i < matrix_size; i++ ) {
        for ( int j = 0; j < matrix_size; j++ ) {
            for ( int k = 0; k < matrix_size; k++ ) {
                C[matrix_size * i + j] += A[matrix_size * i + k] * B[matrix_size * k + j];
            }
        }
    }
    
    return C;
}

/**
 * @description: multiply A * B in the ordinary fashion
 * @param {long int*} A: matrix A 
 * @param {long int*} B: matrix B
 * @param {int} rows: the number of rows of the result matrix
 * @param {int} cols: the number of columns of the result matrix
 * @param {int} M: the number of columns of A, also the number of rows of B
 * @return {long int*} the result of A * B 
 */
long int* sequential_matrix_multiplication(long int* A, long int* B, int rows, int cols, int M) {
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

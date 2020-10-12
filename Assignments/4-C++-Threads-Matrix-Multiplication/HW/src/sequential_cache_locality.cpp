/**
 * @description: multiply A * Bt (B-transpose), after transposing matrix
 * @param {long int*} A: matrix A 
 * @param {long int*} Bt: matrix Bt
 * @param {int} rows: the number of rows of the result matrix
 * @param {int} cols: the number of columns of the result matrix
 * @param {int} M: the number of columns of A, also the number of rows of Bt
 * @return {long int*} the result of A * B 
 */
long int* sequential_matrix_multiplication_plus(long int* A, long int* B, int matrix_size) {

    long int* C = new long int[matrix_size * matrix_size];

    // initialize matrix C
    for ( int i = 0; i < matrix_size * matrix_size; i++ ) {
        C[i] = 0;
    }

    for ( int i = 0; i < matrix_size; i++ ) {
        for ( int j = 0; j < matrix_size; j++ ) {
            for ( int k = 0; k < matrix_size; k++ ) {
                C[matrix_size * i + j] += A[matrix_size * i + k] * B[matrix_size * j + k];
            }
        }
    }

    return C;
}
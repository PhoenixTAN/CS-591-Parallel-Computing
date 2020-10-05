# Cache Locality

Let us explore the importance to performance of cache locality. Implement matrix multiplication (again!) — using the simple sequential algorithm — but in C++ this time.  

Use 1024-by-1024 matrices of integers as inputs.
To simplify the generation of these matrices, populate them with sequential numerals starting at 1 for A and reverse-sequential integers starting at 1024 for B.
Use 1-dimensional (flat) arrays to represent the matrices.

Implement two functions:
- One to multiply A * B in the ordinary fashion.
- One to multiply A * Bt (B-transpose), after transposing matrix B.

Implement a separate function to check the correctness of the transpose-based version by comparing the results of both versions.

After using the test function to check the correctness of your implementations, time the execution of each multiplication function and report the results in your README.  Do not include the transposition of matrix B when timing the second algorithm.  You may transpose the matrix in a separate function whose execution you do not time.  

Do you see an improvement in performance from using the transpose?

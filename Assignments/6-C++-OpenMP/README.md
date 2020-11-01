# README Assignment 6 -- C++ OpenMP

Group 9

Ziqi Tan, U88387934, ziqi1756@bu.edu

Xueyan Xia, U82450191, xueyanx@bu.edu

## Collaboration
Pair discussing, designing, programming and debugging.

## Instructions
Complete Section 6.9, Exercise 1, in C++.

If you believe that OpenMP is applicable to the algorithm, implement the solution using OpenMP.

In your README, state in each case whether OpenMP is applicable.  If it is not applicable, explain—give your reasons.

Try to use good, modern, C++ coding style, and to avoid C-style constructs such as raw pointers and C-style arrays.  Also remember not to follow Schmidt's example regarding the main function—keep your main short and simple, and delegate responsibility to other functions.

## How do we run this program?
For each .cpp file, run this to compile:
```
$ g++ -std=c++14 -fopenmp source_filename.cpp -o your_target_file_name.exe
```
And then run it.

For example, 
```
$ g++ -std=c++14 -fopenmp dense_matrix_multiplication.cpp -o dense_matrix_multiplication.exe
$./dense_matrix_multiplication.exe
```

## Dense Matrix Multiplication
OpenMP is applicable.

The indices are independent.
```C++
// maximum number of threads
const int max_num_of_threads = omp_get_max_threads();
std::cout<< "number of threads: " << max_num_of_threads << std::endl;

int i = 0;
int j = 0;
int k = 0;
long int temp_sum = 0;

// perform matrix multiplication
#pragma omp parallel for private(j) private(k) private(temp_sum) num_threads(max_num_of_threads)
for ( i = 0; i < matrix_size; i++ ) {
    for ( j = 0; j < matrix_size; j++ ) {
        temp_sum = 0;
        for ( k = 0; k < matrix_size; k++ ) {
            temp_sum += A[matrix_size * i + k] * B[matrix_size * k + j];
        }
        C[matrix_size * i + j] = temp_sum;
    }
} 
```

### Result:
```
Running sequential version:
it takes 13.1592 seconds.
============================================
Running openMP version:
number of threads: 12
it takes 2.78932 seconds.
============================================
validating results: true
```

## Pseudo-Polynomial Knapsack Using Dynamic Programming
The inner loop can be parallelized, because index j dose not depend on the the previous index j.

Thus, we can add openMP directives in the inner loop.

```C++
// maximum number of threads
const int max_num_of_threads = omp_get_max_threads();
std::cout<< "number of threads: " << max_num_of_threads << std::endl;

for ( int i = 1; i < N + 1; i++ ) {
    int j = 0;
    #pragma omp parallel for private(j) num_threads(max_num_of_threads)
    for ( j = 0; j < C + 1; j++ ) {
        if ( w[i-1] <= j ) {
            m[AT(i, j)] = MAX(m[AT(i-1, j)], m[AT(i-1, j-w[i-1])] + v[i-1]);
        }
        else {
            m[AT(i,j)] = m[AT(i-1, j)];
        }
    }
}
```

### Result
When both N and C is `1 << 15`, the result is:
```
Running sequential version:
it takes 3.92615 seconds.
============================================
Running openMP version:
number of threads: 12
it takes 2.58614 seconds.
============================================
validating results: true
```

### Analysis
The speed up is greatly depends on the size of `C`. If the size of `C` gets smaller, the speed up will be trivial and even will underperform the sequential version.

Let's see more simulations.

`N` stays `1 << 15`.

`C` decreases to `1 << 14`.

```
Running sequential version:
it takes 1.70447 seconds.
============================================
Running openMP version:
number of threads: 12
it takes 2.12253 seconds.
============================================
validating results: true
```

`C` decreases to `1 << 13`.
```
Running sequential version:
it takes 0.78732 seconds.
============================================
Running openMP version:
number of threads: 12
it takes 1.93415 seconds.
============================================
validating results: true
```

## Repetitive Smoothing of a Vector
How do we parallelize it?

```C++
const int max_num_of_threads = omp_get_max_threads();
std::cout<< "number of threads: " << max_num_of_threads << std::endl;

for (int i = 0; i < M; i++) {

    int k = -2;
    #pragma omp parallel for private(k) num_threads(max_num_of_threads)
    for (int j = 2; j < N - 2; j++) {
        s[j] = 0;
        for (k = -2; k < 3; k++) {
            s[j] += 0.2 * v[j + k];
        }
    }

    #pragma omp parallel for num_threads(max_num_of_threads)
    for (int j = 0; j < N; j++) {
        v[j] = s[j];
    }
}
```

### Result
```
Running sequential version:
it takes 0.123442 seconds.
============================================
Running openMP version:
number of threads: 12
it takes 0.0265839 seconds.
============================================
validating results: true
```


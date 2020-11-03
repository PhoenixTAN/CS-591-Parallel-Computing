

#include <iostream>
#include <chrono>       /* time manipulation */
#include <omp.h>        /* openMP */

/**
 * @description: sequential version of pseudo polynomial knapsack
 * @param {long int* v} a constant and non-negative array of length N
 * @param {int N} the length of array v
*/
long long int sequential_left_fold_of_a_binary_opeartion(long long int* v, int N) {
    long long int result = 1;

    for ( int i = 0; i < N; i++ ) {
        result = result + v[i] + result * v[i];
    }
    
    return result;
}


/**
 * @description: openMP version of pseudo polynomial knapsack
 * @param {long int* v} a constant and non-negative array of length N
 * @param {int N} the length of array v
*/
long long int openMP_left_fold_of_a_binary_opeartion(long long int* v, int N) {

    long long int result = 1;

    #pragma omp declare reduction \
    (custom_op:long long int:omp_out=omp_out + omp_in + omp_out * omp_in) \
    initializer(omp_priv = 0)

    #pragma omp parallel for reduction(custom_op:result)
    for ( int i = 0; i < N; i++ ) {
        result = result + v[i] + result * v[i];
    }
    
    return result;
}

/**
 * @description: validate the result from two matrix multiplication method
 * @return {bool} if the result is correct, return true 
 */
bool validate_result(long int a, long int b) {
    
    std::cout << "validating results: ";

    if ( a != b ) {
        std::cout << "false" << std::endl;
        return false;
    }

    std::cout << "true" << std::endl;
    return true;
}


int main() {

    const int N = 1 << 6;

    // initialization
    long long int* v = new long long int[N];
    long long int result = 0;
    long long int result_openMP = 0;

    // initialize matrix w and v
    srand(time(NULL));  
    for ( int i = 0; i < N; i++ ) {
        v[i] = rand() % 2;
    }

    // time manipulation
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
    std::chrono::duration<double> duration;

    // sequential version
    start_time = std::chrono::steady_clock::now();
    std::cout << "Running sequential version: " << std::endl;
    result = sequential_left_fold_of_a_binary_opeartion(v, N);
    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time;
    std::cout << result << std::endl;
    std::cout << "it takes " << duration.count() << " seconds." << std::endl;
    std::cout << "============================================" << std::endl;
    
    // openMP version
    start_time = std::chrono::steady_clock::now();
    std::cout << "Running openMP version: " << std::endl;
    result_openMP = openMP_left_fold_of_a_binary_opeartion(v, N);
    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time; 
    std::cout << result_openMP << std::endl;
    std::cout << "it takes " << duration.count() << " seconds." << std::endl;
    std::cout << "============================================" << std::endl;

    // validate result
    validate_result(result, result_openMP);

    return 0;
}

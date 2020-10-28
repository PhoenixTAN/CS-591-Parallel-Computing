/*
 * @Author: Ziqi Tan, Xueyan Xia
 * @Description: Assignment 5: C++ Threads and Synchronization
 */

#include <iostream>
#include <chrono>       /* time manipulation */
#include <string>     
#include <stdlib.h>     /* random number */
#include <time.h> 

#include "sequential_maxima.cpp"
#include "parallel_maxima.cpp"


int main () {

    const int num_of_threads = 64;
    const int size_of_array = 1 << 10;

    if ( num_of_threads > size_of_array ) {
        std::cout << "the number of threads is bigger than the size of the array." << std::endl;
        return -1;
    }

    std::cout << "number of threads: " << num_of_threads << std::endl;
    std::cout << "size of array: " << size_of_array << std::endl;

    // time manipulation
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
    std::chrono::duration<double> duration;

    // initialize array
    int* array = new int[size_of_array];
    srand(time(NULL));                    // random number generator
    for ( int i = 0; i < size_of_array; i++ ) {
        array[i] = rand();
    }
    std::cout << "============================================" << std::endl;

    // sequential version
    start_time = std::chrono::steady_clock::now();
    int maxima = get_maxima_sequential(array, size_of_array);
    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time;
    std::cout << "sequential version: " << std::endl;
    std::cout << "maxima: " << maxima << std::endl;
    std::cout << "it takes " << duration.count() << " seconds." << std::endl;
    std::cout << "============================================" << std::endl;
    
    // mutex lock version
    start_time = std::chrono::steady_clock::now();
    maxima = get_maxima_parallel(array, size_of_array, num_of_threads, true);
    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time;
    std::cout << "mutex lock version: " << std::endl;
    std::cout << "maxima: " << maxima << std::endl;
    std::cout << "it takes " << duration.count() << " seconds." << std::endl;
    std::cout << "============================================" << std::endl;

    // atomic data type version
    start_time = std::chrono::steady_clock::now();
    maxima = get_maxima_parallel(array, size_of_array, num_of_threads, false);
    end_time = std::chrono::steady_clock::now();
    duration = end_time - start_time;
    std::cout << "atomic data type version: " << std::endl;
    std::cout << "maxima: " << maxima << std::endl;
    std::cout << "it takes " << duration.count() << " seconds." << std::endl;
    std::cout << "============================================" << std::endl;

    delete[] array;
    return 0;
}

#include <mutex>
#include <vector>
#include <thread>
#include <atomic>   // atomic data types

/**
 * @description: parallel method to get global maxima
 * @param {int*} array: target array
 * @param {int} size_of_array: the size of the target array
 * @param {int} num_of_threads: the number of threads
 * @param {bool} method: if true, use mutex lock. If false, use atomic data type
 * @return {int} the global maxima of the target value
 */
int get_maxima_parallel(int* array, int size_of_array, int num_of_threads, bool method) {

    if ( size_of_array <= 0 ) {
        return -1;
    }

    std::vector<std::thread> threads;
    
    std::atomic<int> atomic_maxima(array[0]);

    std::mutex mutex;
    int maxima = array[0];
    int *maxima_ptr = &maxima;

    // synchronization by mutex lock
    auto worker_with_mutex = [&array, &mutex, maxima_ptr, size_of_array](int start_index, int chunk_size) -> void {

        // get the local maxima
        int local_maxima = array[start_index];
        for ( int i = start_index + 1; i < start_index + chunk_size && i < size_of_array; i++ ) {
            if ( array[i] > local_maxima ) {
                local_maxima = array[i];
            }
        }

        // acquire mutex lock
        const std::lock_guard<std::mutex> lock(mutex);
        // update the global maxima
        if ( local_maxima > *maxima_ptr ) {
            *maxima_ptr = local_maxima;
        }

    };

    // synchronization by a single atomic integer object
    auto worker_with_atomic = [&array, &atomic_maxima, size_of_array](int start_index, int chunk_size) -> void {
        
        // get the local maxima
        int local_maxima = array[start_index];
        for ( int i = start_index + 1; i < start_index + chunk_size && i < size_of_array; i++ ) {
            if ( array[i] > local_maxima ) {
                local_maxima = array[i];
            }
        }

        // update the global maxima
        if ( local_maxima > atomic_maxima ) {
            atomic_maxima = local_maxima;
        }
    };

    // partition
    int chunk_size = size_of_array / num_of_threads;
    for ( int start_index = 0; start_index < size_of_array; start_index += chunk_size ) {
        if ( method ) {
            threads.emplace_back(std::thread(worker_with_mutex, start_index, chunk_size));
        }
        else {
            threads.emplace_back(std::thread(worker_with_atomic, start_index, chunk_size));
        }
    }

    // join threads
    for ( std::thread& thread : threads ) {
        thread.join();
    }

    threads.clear();

    if ( method ) {
        return maxima;
    }

    return atomic_maxima;
}

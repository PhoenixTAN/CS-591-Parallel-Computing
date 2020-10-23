//
//  prefix_sum.cpp
//  PrefixSum
//
//  Created by Amittai Aviram on 10/13/20.
//  Copyright © 2020 Amittai Aviram. All rights reserved.
//

#include <condition_variable>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <string>
#include <vector>


/**
 @file prefix_sum.cpp
 Demonstration of sequential and parallel computations of the prefix sum of a sequence of random integers.
 @author Amittai Aviram
 @date 2020-10-13
 */

/**
 Demonstration of the sequential version and a parallel version of the Prefix Sum algorithm.
 */
class ParallelPrefixSum{
    
public:
    
    /**
     Constructor.
     @param num_nums Size of input sequence of random integers in the range [0, 10).
     @param num_threads Number of concurrent threads used to divide up the work of computing the prefix sum.
     */
    ParallelPrefixSum(uint32_t num_nums, uint32_t num_threads) :
    num_nums{num_nums},
    num_threads{num_threads}
    {
        srand(1);
        for (uint32_t i = 0; i < num_nums; ++i) {
            nums.push_back(rand() % 10);
            check_nums.push_back(nums[i]);
        }
        for (uint32_t i = 0; i < num_threads - 1; ++i) {
            partial_sums.push_back(-1);
            mutexes.emplace_back(new std::mutex());
            condition_variables.emplace_back(new std::condition_variable());
        }
    }
    
    /**
     Run the sequential algorithm in place on the check_nums vector, which is a copy of the nums vector.
     */
    void run_sequential() {
        start = std::chrono::high_resolution_clock::now();
        prefix_sum(check_nums, 0, check_nums.size());
        end = std::chrono::high_resolution_clock::now();
    }
    
    /**
     Run the parallel algorithm.  Create num_thread threads, assign a portion of the input sequence ot each thread,
     and compute the prefix sum.  This computation is in place and changes the contents of the nums sequence.
     */
    void run_parallel() {
        start = std::chrono::high_resolution_clock::now();
        std::vector<std::thread> threads;
        for (uint32_t pid = 0; pid < num_threads; ++pid) {
            threads.emplace_back([this, pid]{this->worker(pid);});
        }
        for (auto& thread : threads) {
            thread.join();
        }
        end = std::chrono::high_resolution_clock::now();
    }
    
    /**
     Computes the time interval between start and end as a floating-point number.
     @return Value in seconds of the time interval between start and end, with a fractional part.
     */
    double get_time() {
        std::chrono::duration<double> elapsed = end - start;
        return elapsed.count();
    }

    /**
     Report whether the parallel version of the algorithm puts out the same results as the sequential version.
     Returns true if the results are equal and false otherwise.
     */
    bool verify() {
        for (uint64_t i = 0; i < num_nums; ++i) {
            if (nums[i] != check_nums[i]) {
                return false;
            }
        }
        return true;
    }
    
    /**
     Print the sequence of random numbers in its current state, which may be before or after the prefix sum computation changes it.
     */
    void print_nums() {
        for (auto num : nums) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
    
private:
    
    void prefix_sum(std::vector<int64_t>& nums, uint64_t start, uint64_t end) {
        for (uint64_t i = start + 1; i < end; ++i) {
            nums[i] += nums[i - 1];
        }
    }
    
    void worker(uint32_t pid) {
        uint64_t chunk = num_nums / num_threads;
        uint64_t start = pid * chunk;
        uint64_t end = start + chunk;
        if (pid == num_threads - 1) {
            end = num_nums;
        }
        prefix_sum(nums, start, end);
        int64_t carried_sum = 0;
        if (pid > 0) {
//            std::unique_lock<std::mutex> lock(mutex);
            std::unique_lock<std::mutex> lock(*(mutexes[pid - 1]));
//            condition_variable.wait(lock, [&]{return partial_sums[pid - 1] >= 0;});
            condition_variables[pid - 1]->wait(lock, [&]{return partial_sums[pid - 1] >= 0;});
            carried_sum += partial_sums[pid - 1];
        }
        if (pid < num_threads - 1) {
            std::unique_lock<std::mutex> lock(*(mutexes[pid]));
            partial_sums[pid] = carried_sum + nums[end - 1];
            lock.unlock();
            condition_variables[pid]->notify_one();
        }
        if (pid > 0) {
            for (uint64_t i = start; i < end; ++i) {
                nums[i] += carried_sum;
            }
        }
    }
    
    const uint32_t num_threads;
    const uint32_t num_nums;
    std::vector<int64_t> nums;
    std::vector<int64_t> check_nums;
    std::vector<int64_t> partial_sums;
    std::vector<std::unique_ptr<std::mutex>> mutexes;
    std::vector<std::unique_ptr<std::condition_variable>> condition_variables;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
};


/**
 - Create a ParallelPrefix Sum object.
 - Print the sequence of random numbers before any computation.
 - Compute the prefix sum using the parallel version fo the algorithm.  This changes the contents of the number sequence.
 - Print the number sequence again, showing the prefix sum results.
 - Check this result against the results of sequential computation.
 - Report the results of the verification step.
 */
int main(int argc, const char * argv[]) {
    const uint32_t log_num_nums = 22;
    const uint32_t num_threads = 4;
    const uint32_t num_nums = 1 << log_num_nums;
    ParallelPrefixSum pps(num_nums, num_threads);
    pps.run_sequential();
    const double sequential_time = pps.get_time();
    pps.run_parallel();
    const double parallel_time = pps.get_time();
    const double speedup = sequential_time / parallel_time;
    std::string correct = pps.verify() ? "" : "in";
    std::cout << "Results are " << correct << "correct." << std::endl;
    std::cout << "Sequential execution time: " << sequential_time << " seconds." << std::endl;
    std::cout << "Parallel execution time: " << parallel_time << " seconds." << std::endl;
    std::cout << "Speedup: " << speedup << "." << std::endl;
    return 0;
}

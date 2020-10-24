# Assignment 5: C++ Threads and Synchronization


Complete Schmidt, et al., Section 5.5, Exercise 1, as follows:

1. Implement the function for finding the local maximum for each thread's chunk.  You will use this code twice — avoid duplication.  (You may use a boolean flag, enumeration type parameter, or function pointer parameter to tell this function which method to use to update the global maximum.)
2. Implement two different functions for finding the global maximum among the local maxima using each of these techniques to update the global maximum:
    - Mutex locks
    - A single atomic integer object.
3. Implement a verification function that uses the sequential algorithm to find the global maximum.
4. In main
    - Time execution for each of the two methods of computing the global maximum.
    - Check the two results from the two different methods against the result of sequential execution for correctness.
    - Report the two execution times and the result of the correctness check to stdout.  (Of course, if your verification step fails, fix your code!)
5. Report the timing results in your README, along with who contributed what, as described in the Assignment Requirements document.

Write an efficient parallel program that computes the maximum element in an array of n floats. Initially, partition the array into equally sized portions and concurrently perform a local max-reduction on each fragment. Subsequently, merge the partial results using either locks or atomics. Compare the execution times. Which approach performs better?

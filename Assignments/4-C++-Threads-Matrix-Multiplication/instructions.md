# Assignment 4 --- C++ Threads
In this assignment, you will implement parallel matrix multiplication yet again, as in Homework 3, but using the C++ STL thread library.  You will use only one size for the matrices, but provide for a four-way comparison:

1. Sequential execution of the standard algorithm (without threads).
2. Sequential execution of an implementation using the transpose of the second matrix to improve cache locality, as presented in Schmidt et al.,
3. Parallel execution with normal matrices, using 8 threads and 8 tiles. The partitioning (tiling) logic should be the same as for Homework 1 as I presented it in my "tips" slides (and as most of you implemented it in your homework). For greater efficiency, please move the logic of determining the partition for each thread (vertical start, vertical end, horizontal start, and horizontal end) to the sequential portion of your algorithm before launching your threads.
4. Parallel execution (as above), but using the transpose of the second matrix.

Measure the execution times of each version and report them in your README file.  Be sure to include instructions in your README file for compiling and running your code.


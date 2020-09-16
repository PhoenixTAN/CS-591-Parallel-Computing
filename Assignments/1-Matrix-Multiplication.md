# Assignment 1 --- Matrix Multiplication

Implement parallel matrix multiplication of square matrices of 2^{2d} random integers, using 2^k processes (i.e.., both matrix size and number of processes should be powers of 2—d is the log2 of the number or rows/columns).  

Compute the theoretical speedup with 1, 2, 4, and 8 processes, respectively (k = 0, 1, 2, and 3).  

Test your program with various matrix sizes and numbers of workers and measure the actual running times. 

Try to find the best performance (even if it is slower than the sequential version).  

Compute speedup and efficiency for the best combination of matrix size and number of workers.  Repeat this exercise for both threads (at the user level) and processes (at the kernel level).  

Report your results in a README text file.

This assignment is to be completed by a team of 2 students.  You are free to collaborate in whatever way you wish, but both of your names should appear in the README file.

Code must have complete docstrings, explaining the behavior of functions and the respective meanings of function parameters and return values.  You must also use type hints on all symbols that allow them.

## Details
1. Set your random seed to 1 so as to have a reproducible sequence of pseudo-random integers.
2. Your principal function should take k as one of its parameters.
3. You will need to work out some iteration logic to "chunk" your matrix appropriately into 2, 4, or 8 equal segments.  (Look for tips in class.)
4. You will need to use both the threading and multiprocessing Python libraries for this exercise.  Make separate versions, one using each of these libraries.
5. Use shared memory.  For the process-based version you will find the multiprocessing. Array class useful for this purpose.
6. Use any Python construct for creating and destroying the threads and processes.  You will likely find the lower-level Thread and Process classes the most useful for this assignment.
7. Test your program out on a CS Linux machine.  If you do not yet have an account on one, please be sure to acquire one by registering here.

https://www.bu.edu/cs/account/

Remember the final exam in CS 562? MapReduce for matrix multiplication.



# Assignment 8: CUDA programming
Please complete Exercise 1 in Schmidt, Chapter 7, Section 6 (page 284).  

Write your solution as a working CUDA program.  Include a host function to check the results using sequential computation, and compare the running times of the CUDA parallel and host sequential functions.  

Report the speedup and efficiency to stdout and in your README.

You should be able to run your code from your home directory on the SCC.

Write a CUDA program that computes the sum of two vectors x, y ∈ Rn stored in the host arrays
float * x and float * y. Compare the execution times of the corresponding kernel and memory transfers with a sequential program running on a single CPU core. Is your CUDA parallelization beneficial?


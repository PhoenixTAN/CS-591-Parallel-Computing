# CS 591 Parallel Computing and Programming Syllabus

## Description
This course is an introduction to the theory, techniques, and practices of parallel computing.  We will explore the principle models of parallel computation, such as **distributed and shared-memory, threads, pipelines, and message passing**, as well as the challenges these approaches pose, and known methods for addressing them.  

We will consider **various systems and settings** that support parallel computation, **from multi-core architectures and other hardware devices to distributed systems**.  

Finally, we will get acquainted with what parallel programming allows us to do: the design and analysis of distinctively parallel algorithms, their benefits, and their limitations.  Students will prepare programming exercises, including the simulation of distributed systems, as well as a team-based final project.  Students will need to know, or to learn, Python 3 and  modern C++.

## Learning Objectives
- Knowledge of concepts used to analyze and evaluate parallel programs and systems, such as speedup, scalability, efficiency, Amdahl's Law, and Gustafson's Law.
- Knowledge of techniques for managing shared and distributed memory systems, including synchronization primitives and network topologies.
- Familiarity with parallel programming constructs and techniques in Python 3 and modern C++:
    - Threads and processes
    - OpenMP
    - Compute Unified Device Architecture (CUDA) for execution on graphic processing units (GPUs)
    - Message passing using MPI for execution on distributed systems.
- Familiarity with important parallel algorithms and the design and analysis of parallel algorithms.

## Required Materials
Required Textbook: Bertil Schmidt, Jorge González-Domínguez, and Chrstian Hundt, Parallel Programming Concepts and Practice

(Cambridge, MA: Elsevier-Morgan Kaufman, 2018).

Optional: Changkun Ou, Modern C++ Tutorial: C++ 11/17/20 On the Fly

## Lessons

### Week 1 -- Introduction to Parallel Programming

#### Conceptual basics of parallel programming
1. Parallelism in every day life
2. Patterns of parallel work
3. Mapping to machines
4. Example: merging k arrays
5. Speedup
#### Parallel computing systems
1. Independence and communication
2. Example: summing
3. Scaling and efficiency


### Week 2 -- Distributed- and Shared-Memory Systems
#### Class
1. Review of speedup, scaling, and efficiency
2. The Parallel Random-Access Machine (PRAM) theoretical model
3. Prefix sum algorithm.
4. Group programming: parallel prefix sum (in Python 3).

#### Homework
due before class on Tuesday, 22 September 2020:

1. Reading: Parallel Programming Concepts and Practice, Chapter 2.
2. Programming: Implement parallel array merging of k arrays in Python 3, allowing the user to set the number of parallel threads.  The program should support an arbitrary number k of arrays.  Compute the theoretical speedup and record your work in a README text file. Test with k = 8 arrays and 1024 random integers per array, with 2, 4, and 8 threads, and report the actual speedups in your README.




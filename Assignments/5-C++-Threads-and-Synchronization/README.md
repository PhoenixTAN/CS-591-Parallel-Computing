# README
Assignment 5: C++ Threads and Synchronization

Group 9

Ziqi Tan, U88387934, ziqi1756@bu.edu

Xueyan Xia, U82450191, xueyanx@bu.edu

## Collaboration
Pair discussing, designing, programming and debugging.

## File structure
- src
    - main.cpp
    - sequential_maxima.cpp
    - parallel_maxima.cpp
- README.md

## How to run this program
1. Open a terminal and enter /src folder. Compile the program with this command.
```
$ g++ -o main.exe main.cpp
```
2. Run.
```
$ ./main.exe
```

If you want to change the number of threads and the size of the array, change them at the very beginning of the main function of main.cpp.

## Results
We use 8 threads throughput this experiment, because we need as many as threads to show the cost of the acquire and release of a mutex lock. Besides, the size of the array cannot be too big, because the time you take to iterate part of the array will become longer.

```
number of threads: 8
size of array: 8
============================================
sequential version:
maxima: 32037
it takes 1e-07 seconds.
============================================
mutex lock version:
maxima: 32037
it takes 0.0009573 seconds.
============================================
atomic data type version:
maxima: 32037
it takes 0.0008297 seconds.
============================================
```

```
number of threads: 8
size of array: 1024
============================================
sequential version:
maxima: 32732
it takes 1.9e-06 seconds.
============================================
mutex lock version:
maxima: 32732
it takes 0.0006295 seconds.
============================================
atomic data type version:
maxima: 32732
it takes 0.0004196 seconds.
============================================
```

```
number of threads: 8
size of array: 32768
============================================
sequential version:
maxima: 32765
it takes 6.7e-05 seconds.
============================================
mutex lock version:
maxima: 32765
it takes 0.0006049 seconds.
============================================
atomic data type version:
maxima: 32765
it takes 0.0004369 seconds.
============================================
```

```
number of threads: 8
size of array: 1048576
============================================
sequential version:
maxima: 1072038548
it takes 0.0022644 seconds.
============================================
mutex lock version:
maxima: 1072038548
it takes 0.0011613 seconds.
============================================
atomic data type version:
maxima: 1072038548
it takes 0.0008627 seconds.
============================================
```

```
number of threads: 8
size of array: 33554432
============================================
sequential version:
maxima: 1073479687
it takes 0.0716747 seconds.
============================================
mutex lock version:
maxima: 1073479687
it takes 0.0167979 seconds.
============================================
atomic data type version:
maxima: 1073479687
it takes 0.0176487 seconds.
============================================
```
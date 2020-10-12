# README

Group 9

Ziqi Tan, U88387934, ziqi1756@bu.edu

Xueyan Xia, U82450191, xueyanx@bu.edu

## Collaboration
Pair discussing, designing, programming and debugging.

## File structure

- src
    - main.cpp
    - multithreading.cpp
    - multithreading_cache_locality.cpp
    - sequential_cache_locality.cpp
    - sequential.cpp
    - utils.cpp
- README.md

## How to run this program
1. Open a terminal and enter /src folder. Compile the program with this command.
```
$ g++ -o main.exe main.cpp
```
2. Type in one of the following parameters and run the program. 
```
$ ./main.exe sequential_plus
$ ./main.exe parallel
$ ./main.exe parallel_plus
```

## Results
The result is based on matrix size = 1024 * 1024, number of threads = 8.

- sequential_plus  
Utilization of cache locality for matrix multiplication without multithreading.
```
ordinary fashion takes 16.9963 seconds.
new method takes 3.5244 seconds.
```

- parallel  
Solve problem of matrix multiplication with assigned number of threads without utilization of cache locality. 
```
ordinary fashion takes 16.6175 seconds.
new method takes 3.22202 seconds.
```

- parallel_plus  
Utilize cache locality to solve problem of matrix multiplication with assigned number of threads.
```
ordinary fashion takes 16.5565 seconds.
new method takes 1.23056 seconds.
```


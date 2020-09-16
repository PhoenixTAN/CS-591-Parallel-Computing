# Introduction

The goal of this chapter is to provide an overview of introductory concepts and terminologies in parallel
computing.

We start with learning about **speedup, efficiency, cost, scalability, and the computation-to-communication ratio** by analyzing a simple yet instructive example for summing up numbers using a varying number of processors.

We get to know about the **two most important parallel architectures: distributed memory systems and shared memory systems**.

## Contents
1. Motivational Example and Its Analysis
    1. The General Case and the Computation-to-Communication Ratio
2. Parallelism Basics
    1. Distributed Memory System
    2. Shared Memory Systems
3. HPC Trends and Rankings
4. Additional Exercises

## Motivational Example and Its Analysis

### Terminologies

#### Speedup
You have designed a parallel algorithm or written a parallel code. Now you want to
know how much faster it is than your sequential approach; i.e., you want to know the speedup.

The speedup (S) is usually measured or calculated for almost every parallel code or algorithm and is simply defined as the quotient of the time taken using a single processor (T (1)) over the time measured using p processors (T (p)) .
$$
S = \frac{T(1)}{T(p)}
$$

#### Efficiency and cost

The best speedup you can usually expect is a linear speedup; i.e., the maximal speedup you can achieve with p processors or cores is p (although there are exceptions to this, which are referred to as super-linear speedups).

The Efficiency E measures exactly that by dividing S by P; i.e., linear speedup would then be expressed by a value close to 100%.
$$
E = \frac{S}{P} = \frac{T(1)}{T(p)\times p}
$$
The cost C is similar but relates the runtime T (p) (instead of the speedup) to the number of utilized processors (or cores) by multiplying T (p) and p.
$$
C = T(p) \times p
$$

#### Scalability

Often we do not only want to measure the efficiency for one particular number of processors or cores but for a varying number; e.g. P = 1, 2, 4, 8, 16, 32, 64, 128, etc. This is called scalability analysis and **indicates the behavior of a parallel program when the number of processors increases**. 

Besides varying **the number of processors**, the **input data size** is another parameter that you might want to vary when executing your code. Thus, there are two types of scalability: **strong scalability** and **weak scalability**. 

##### Strong scalability

We measure efficiencies for a varying number of processors and **keep the input data size fixed**.

##### Weak scalability

Weak scalability shows the behavior of our parallel code for varying both the number of processors and the input data size; i.e. **when doubling the number of processors we also double the input data size**.

#### Computation-to-communication ratio

This is an important metric influencing the achievable scalability of a parallel implementation. It can be defined as **the time spent calculating divided by the time spent communicating messages between processors**. A higher ratio often leads to improved speedups and efficiencies.

### Example --- Summing 1024 Numbers

Given: a distributed system of interconnected processors.
We hold the input size constant so as to measure strong scalability.

####  Assumptions:

1. Each processor takes 1 time unit to add two numbers.
2. Each communication between processors takes 3 time units.
3. We have  p = 2^k processors (for some small integer k).

#### Algorithm: Distributed Divide-and-Conquer

1. The first node divides the input list in half and sends one half to its neighbor.
2. This division is repeated recursively until each of the processors receives an even share of the input.
3. After dividing up the input, the nodes must gather the results recursively back from their neighbors until the total winds up in node 0.

![summation](.\images\summation.png)

#### Cost Comparisons

Example with n = 1024
$$
p = 1 \\
T(1, n) = n - 1 = 1023 \\
$$

$$
p = 2 \\
T(2, n) = 3 \times 1 + n / 2 + 3 + 1 = 517 \\
S = 1023/517 = 1.98 \\
E = \frac{1023}{517 \times 2} = 99\%
$$

$$
p = 4\\
T(4, n) = 3 + 3 + 1024/4-1 + 3 + 1 + 3 + 1 = 269\\
S = 1023/269 = 3.80\\
E = \frac{1023}{269\times 4} = 95\%
$$

$$
p=8\\
T(8,n)=3+3+3+n/8−1+3+1+3+1+3+1=148\\
S=1023/148=6.91\\
E=\frac{1023}{148\times 8}=86\%
$$

$$
T(p,n)=T(2^q,2^k)=3q+2^{k−q}−1+3q+q=2^{q−k}−1+7q
$$

Components:

1. Data distribution time
2. Local sum computation time
3. Time to collect partial results
4. Time to add partial results locally

#### Strong scalability analysis

![strong-scalability-analysis](.\images\strong-scalability-analysis.png)

随着处理器数量的增加，效率显著下降，说明这个算法并不是Strong scalability.

#### Weak scalability analysis

![weak-scalability-analysis](.\images\weak-scalability-analysis.png)

随着处理器数量和输入规模的同时增长，效率没有下降很多，所以我们的算法是weak scalability.

## Parallelism basics

### Distributed Memory Systems

Each *processing element* PE in our parallel summation algorithm in the previous section **has only access to its own local memory**. Access to data stored in the memory of another PE needs to be implemented by **an explicit communication step**. This type of parallel computer architecture is called a **distributed memory system**.

![distributed-memory-system](.\images\distributed-memory-system.png)

All CPUs (or nodes or PEs) are connected through an **interconnection network**. Each CPU can only operate on data stored in its local memory. Remote data accesses need to be explicitly implemented through message passing over the interconnection network.

### Shared Memory Systems

![shared-memory-system](.\images\shared-memory-system.png)
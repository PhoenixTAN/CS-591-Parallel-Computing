from multiprocessing import Process, Array, Lock
import random
from typing import List
import time


def brute_force_matrix_multiplication(A: List[List[int]], B: List[List[int]]):
    
    print('brute forcing...')
    N = len(A)
    C = []

    for i in range(0, N):
        row = []
        for j in range(0, N):
            res = 0
            for k in range(0, N):
                res += A[i][k] * B[k][j]
            row.append(res)
        C.append(row)

    return C


def isSameMatrix(A: List[int], B: List[int]) -> bool: 

    if len(A) != len(B):
        return False
    for i in range(len(A)):
        if A[i] != B[i]:
            return False

    return True


def parallel_matrix_multiplication(A: List[int], B: List[int], lock, matrix_size: int, num_of_processors: int):
    '''
        param A: the flat matrix
        param B: the flat matrix
        param lock: a lock for matrix C
        param matrix_size: the size of the matrix
        param num_of_processors: the number of processors
    '''

    # we can only use matrix_size * matrix_size processors at most
    N = matrix_size * matrix_size
    num_of_processors = min(num_of_processors, N)
    print('actual number of processors: ', num_of_processors)

    # initialize process pool
    processes = [None] * num_of_processors

    # matrix A, B and C can be shared by different workers
    shared_A = Array('i', N)
    shared_B = Array('i', N)
    shared_C = Array('i', N)

    for i in range(0, N):
        shared_A[i] = A[i]
        shared_B[i] = B[i]

    # partition the matrix A, and dispatch the jobs to workers
    interval = N // num_of_processors
    index = 0
    process_id = 0
    while index < N:
        processes[process_id] = Process(
            target=worker, 
            args=(process_id, shared_A, shared_B, shared_C, 
                    lock, index, matrix_size, num_of_processors
            )
        )
        index += interval
        process_id += 1
        
    for process in processes:      
        process.start()

    for process in processes:
        process.join()

    return shared_C


def worker(process_id: int, A: List[int], B: List[int], C: List[int], 
            lock, start: int, matrix_size: int, num_of_processors: int) -> None:
    '''
        param process_id: an integer for test purpose
        param start: start index in matrix A
        param end: end index in matrix A
        param lock: used for synchronization
        param matrix_size: size of matrix
        param num_of_processors: the number of processors
    '''

    N = matrix_size * matrix_size

    # calculate the size of the temporary result
    num_of_rows = max(1, matrix_size // num_of_processors)
    num_of_columns = matrix_size
    
    # initialize a temporary result matrix C
    temp_C = [0] * (num_of_rows * num_of_columns)

    # used for the inner iteration
    chunk_size = min(matrix_size, N // num_of_processors)

    # row and col iterate the temporary matrix C
    for row in range(num_of_rows):
        for col in range(num_of_columns):
            # iterate a chunk
            for k in range(chunk_size):
                # temp_C[row][col] += A[row][k] * B[k][col]
                # we need to reflect this index to one dimension
                index_of_A = row * matrix_size + k + start
                index_of_B = (k + start % matrix_size) * matrix_size + col
                index_of_temp_C = row * matrix_size + col
                temp_C[index_of_temp_C] += A[index_of_A] * B[index_of_B]
              
    # actual row start index in C
    start_row_of_C = start // matrix_size
    
    # C is shared memory
    lock.acquire()      # acquire lock before enterting critical section
    for i in range(len(temp_C)):
        C[i + start_row_of_C * matrix_size] += temp_C[i]
    lock.release()      # release lock when leaving critical section

    return None



if __name__ == '__main__':

    lock = Lock()   # lock for shared resource
    random.seed(1)

    # try different size of matrix
    for d in range(0, 8):
        matrix_size = 2 ** d
        print("matrix size: ", matrix_size)
        A = []
        B = []
        # generate random matrice
        for i in range (0, matrix_size):
            new_list = random.sample(range(0, 1000), matrix_size)
            A.append(new_list)
            new_list = random.sample(range(0, 1000), matrix_size)
            B.append(new_list)

        start = time.time()
        C = brute_force_matrix_multiplication(A, B)
        end = time.time()
        print('brute force running time: ', end - start)

        # flatten matrix A and B
        flat_A = [val for row in A for val in row]
        flat_B = [val for row in B for val in row]

        run_time = []

        # different number of processors
        for k in range(0, 4):
            num_of_processors = 2 ** k
            print('number of processors: ', num_of_processors)

            print('parallel matrix multiplication... ')
            start = time.time()
            result = parallel_matrix_multiplication(flat_A, flat_B, lock, matrix_size, num_of_processors)
            end = time.time()
            print('multiprocessing running time: ', end - start)
            run_time.append((min(num_of_processors, matrix_size * matrix_size), end - start))

            print('validating the result...')
            flat_C = [val for row in C for val in row]
            print(isSameMatrix(flat_C, result))
            print()

        print('summary')
        print('matrix size: ', matrix_size)
        print('run time', run_time)
        
        speed_up = []
        efficiency = []
        for i in range(len(run_time)):
            num_of_processors = run_time[i][0]
            amount_of_time = run_time[i][1]
            speed_up.append(run_time[0][1] / amount_of_time)
            efficiency.append(run_time[0][1] / amount_of_time / num_of_processors)
        
        print('speed up: ', speed_up)
        print('efficiency: ', efficiency)
        print()


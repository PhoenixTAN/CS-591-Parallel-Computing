from multiprocessing import Array, Process, Queue
import random
from timeit import default_timer as timer

# Implementation of the Parallel Prefix Sum algorithm using a standalone function.


def prefix_sum(pid: int, nums: Array, receive_queue: Queue, send_queue: Queue, num_processes: int):
    """
    Implements the parallel prefix sum algorithm:
    Step 1: Perform the local prefix sum on the current process's chunk.
    Step 2: Receive the last sum value from the process to the "left."
            Add this value to the local last sum value and pass that to the process to the "right."
    Step 3: Add the sum received from the "left" to each element of the current chunk.
    :param pid: Unique identifier of the current process--an integer.
    :param nums: Array of integers to serve as the input.
    :param receive_queue: Queue from which to receive the carried sum from the process to the "left."
    :param send_queue: Queue on which to end a new carried sum to the process to the "right."
    :param num_processes: Number of processes among which the program divides the work.
    :return:
    """
    chunk_size = len(nums) // num_processes
    start = pid * chunk_size
    end = start + chunk_size
    # Step 1: Compute prefix sum over chunk.
    for i in range(start + 1, end):
        nums[i] += nums[i - 1]
    # If there is only one process, we are done.
    if num_processes == 1:
        return
    # Step 2: Receive the sum carried from the process to the "left," if one exists.
    # Add this to the current final sum.
    # Pass this aggregate sum to the process to the "right," if one exists.
    carried_sum = 0
    # For all but the "leftmost" process, receive a sum carried from the "left."
    if pid > 0:
        carried_sum += receive_queue.get()
    # For all but the "rightmost" process, send the next sum to the process to the "right."
    if pid < num_processes - 1:
        send_queue.put(carried_sum + nums[end - 1])
    # Add the received carried sum, if one was received, to all the elements in the current chunk.
    if pid > 0:
        for i in range(start, end):
            nums[i] += carried_sum


if __name__ == '__main__':
    n = 2 ** 10
    p = 4
    random.seed(1)
    nums_list = random.choices(range(10), k=n)
    nums = Array('i', nums_list)
    nums0 = Array('i', nums_list)
    # print(nums0[:])
    start = timer()
    prefix_sum(0, nums0, None, None, 1)
    end = timer()
    # print(nums0[:])
    print('Sequential time: ', end - start)

    # Each queue connects the process at its index (pid) to
    # the process at index pid + 1.
    # Each process receives from the queue bearing its index.
    # Each process sends on the queue bearing the next index.
    queues = []
    for i in range(p):
        queues.append(Queue())
    processes = []
    for i in range(p):
        q0 = queues[i] if i > 0 else None
        q1 = queues[i + 1] if i < (p - 1) else None
        processes.append(Process(target=prefix_sum, args=(i, nums, q0, q1, p)))
    # print(nums[:])
    start = timer()
    for process in processes:
        process.start()
    for process in processes:
        process.join()
    end = timer()
    # print(nums[:])
    print('Parallel time: ', end - start)

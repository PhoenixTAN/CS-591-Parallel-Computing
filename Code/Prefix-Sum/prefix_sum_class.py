from multiprocessing import Array, Condition, Process
import random
from timeit import default_timer as timer


class PrefixSumCalculator:
    """
    Class implementing the Parallel Prefix Sum algorithm, using a member Array to hold
    the processes' respective sums, synchronized to control reading.
    """

    def __init__(self, num_values: int, num_processes: int):
        """
        Initializes the member Arrays to hold the input numbers,
        the sums, and the ready signals, as well as a list of condition variables.
        :param num_values: Size of the desired list of random numbers to serve as input.
        :param num_processes: Number of processes among which to devide the work.
        """
        self.num_values = num_values
        self.num_processes = num_processes
        random.seed(1)
        self.nums_list = random.choices(range(10), k=self.num_values)
        self.nums = Array('i', self.nums_list)
        self.sums = Array('i', self.num_processes)
        self.ready = Array('b', [False] * num_processes)
        self.condition_variables = []
        for i in range(num_processes):
            self.condition_variables.append(Condition())

    def prefix_sum(self, pid: int):
        """
        Implements the parallel prefix sum algorithm:
        Step 1: Perform the local prefix sum on the current process's chunk.
        Step 2: Receive the last sum value from the process to the "left."
        Add this value to the local last sum value and pass that to the process to the "right."
        Step 3: Add the sum received from the "left" to each element of the current chunk.
        :param pid: Unique identifier of the current process--an integer.
        """
        chunk_size = self.num_values // self.num_processes
        start = pid * chunk_size
        end = start + chunk_size
        # Step 1: Compute prefix sum over chunk.
        for i in range(start + 1, end):
            self.nums[i] += self.nums[i - 1]
        # If there is only one process, we are done.
        if self.num_processes == 1:
            return
        # Step 2: Receive the sum carried from the process to the "left," if one exists.
        # Add this to the current final sum.
        # Pass this aggregate sum to the process to the "right," if one exists.
        carried_sum = 0
        # For all but the "leftmost" process, receive a sum carried from the "left."
        if pid > 0:
            with self.condition_variables[pid]:
                self.condition_variables[pid].wait_for(lambda: self.ready[pid])
                carried_sum += self.sums[pid]
        # For all but the "rightmost" process, send the next sum to the process to the "right."
        if pid < self.num_processes - 1:
            with self.condition_variables[pid + 1]:
                self.sums[pid + 1] = carried_sum + self.nums[end - 1]
                self.ready[pid + 1] = True
                self.condition_variables[pid + 1].notify()
        # Add the received carried sum, if one was received, to all the elements in the current chunk.
        if pid > 0:
            for i in range(start, end):
                self.nums[i] += carried_sum

    def run(self):
        processes = []
        for i in range(self.num_processes):
            processes.append(Process(target=self.prefix_sum, args=(i,)))
        for process in processes:
            process.start()
        for process in processes:
            process.join()


if __name__ == '__main__':
    num_values = 2 ** 5
    num_processes = 4

    prefix_sum_calculator = PrefixSumCalculator(num_values, 1)
    start = timer()
    prefix_sum_calculator.prefix_sum(0)
    end = timer()
    print(prefix_sum_calculator.nums[:])
    print('Sequential time: ', end - start)

    prefix_sum_calculator_p = PrefixSumCalculator(num_values, num_processes)
    print(prefix_sum_calculator_p.nums[:])
    # processes = []
    # for i in range(num_processes):
    #     processes.append(Process(target=prefix_sum_calculator_p.prefix_sum, args=(i,)))
    start = timer()
    prefix_sum_calculator_p.run()
    # for process in processes:
    #     process.start()
    # for process in processes:
    #     process.join()
    end = timer()
    print(prefix_sum_calculator_p.nums[:])
    print('Parallel time: ', end - start)

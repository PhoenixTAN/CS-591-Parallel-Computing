from multiprocessing import Process, Array
import random
from typing import List


def merge_lists(inputs: List[List[int]]) -> List[int]:
    """
    Merges an arbitrary number of sorted lists of integers
    into a single sorted list of integers.
    Iterates over the input list of lists.
    On each iteration, selects pairs (a, b) of lists to merge,
    copying the merged list over list a.
    The outer loop doubles the value of leap starting at 1
    and ending where leap equals the number of input lists.
    The inner loop then finds pairs (i, i + leap), where
    i ranges from 0 to num_lists, incrementing by 2 * leap.
    For k = 8 input lists, the pairs of list indices used for merging
    across all outer loop iterations would be
    (0, 1) -> 0, (2, 3) -> 2, (4, 5) -> 4, (6, 7) -> 6
    (0, 2) -> 0, (4, 6) -> 4
    (0, 4) -> 0
    This routine runs the merges at each outer loop iteration in parallel.
    It creates a new process for each pair in the inner loop.
    It then starts all such processes, joins them to the main process,
    and copies back the results from each merge onto the first of the two
    lists in each pair.

    :param inputs: list of sorted lists of integers
    :return sorted list of integers
    """
    num_lists = len(inputs)
    leap = 1
    while leap < num_lists:
        i = 0
        arrays = [None] * num_lists
        processes = [None] * num_lists
        while i < num_lists:
            if i + leap < num_lists:
                arrays[i] = Array('i', len(inputs[i]) + len(inputs[i + leap]))
                processes[i] = Process(target=merge_two_lists, args=(inputs[i], inputs[i + leap], arrays[i]))
            i += leap * 2
        for process in processes:
            if process is not None:
                process.start()
        for i in range(len(processes)):
            if processes[i] is not None:
                processes[i].join()
                inputs[i] = arrays[i][:]
            processes[i] = None
            arrays[i] = None
        leap *= 2
    return inputs[0]


def merge_two_lists(list_a: List[int], list_b: List[int], result: Array):
    """
    Merges the integers in list_a and list_b into a single Array, provided
    as an out-parameter.
    :param list_a:  list of integers to merge with those in list_b into a single list
    :param list_b:  list of integers to merge with those in list_a into a single list
    :param result:  (out-parameter) shared memory Array, initialized to 0, to hold the merged values
    """
    i = j = k = 0
    end_a = end_b = False
    while i < len(list_a) or j < len(list_b):
        if end_b or (not end_a and list_a[i] < list_b[j]):
            result[k] = list_a[i]
            i += 1
            if i == len(list_a):
                end_a = True
        else:
            result[k] =list_b[j]
            j += 1
            if j == len(list_b):
                end_b = True
        k += 1

if __name__ == '__main__':
    input_lists = []
    input_sizes = [5, 4, 3, 6, 7, 4, 5, 2]
    random.seed(1)
    for input_size in input_sizes:
        new_list = random.sample(range(0, 100), input_size)
        new_list.sort()
        input_lists.append(new_list)
    print('Inputs:')
    for input_list in input_lists:
        print(input_list)
    output_list = merge_lists(input_lists)
    print()
    print('Output:')
    print(output_list)



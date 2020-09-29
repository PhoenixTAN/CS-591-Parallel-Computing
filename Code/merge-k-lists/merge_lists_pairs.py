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

    :param inputs: list of sorted lists of integers
    :return sorted list of integers
    """
    num_lists = len(inputs)
    leap = 1
    while leap < num_lists:
        i = 0
        while i < num_lists:
            if i + leap < num_lists:
                inputs[i] = merge_two_lists(inputs[i], inputs[i + leap])
            i += leap * 2
        leap *= 2
    return inputs[0]


def merge_two_lists(list_a: List[int], list_b: List[int]) -> List[int]:
    """
    Helper function to merge elements of list_a and list_b into
    a single list, which it returns.

    :param list_a:  list of integers to merge with those in list_b into a single list
    :param list_b:  list of integers to merge with those in list_a into a single list
    :return:
    """
    output = []
    while len(list_a) or len(list_b):
        if len(list_b) and (not len(list_a) or list_b[0] < list_a[0]):
            output.append(list_b[0])
            list_b.pop(0)
        else:
            output.append(list_a[0])
            list_a.pop(0)
    return output


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
    print('Outputs:')
    print(output_list)

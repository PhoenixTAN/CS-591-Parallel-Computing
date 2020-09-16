import random
from typing import List


def merge_lists(inputs: List[List[int]]) -> List[int]:
    """
    Merges an arbitrary number of sorted lists of integers
    into a single sorted list of integers.
    Follows a simple algorithm of successively finding the minimum
    of all least (front) values in all input lists and moving that
    entry to the output list, so that the next-smallest value is now
    at the front of that list.  When a list is empty, removes
    that list from the input list of lists.  When the input list of
    lists is empty, the algorithm finishes.

    :param inputs: list of sorted lists of integers
    :return sorted list of integers
    """
    output = []
    while len(inputs):
        next_item = inputs[0][0]
        min_index = 0
        for i in range(1, len(inputs)):
            if inputs[i][0] < next_item:
                next_item = inputs[i][0]
                min_index = i
        output.append(next_item)
        inputs[min_index].pop(0)
        if not len(inputs[min_index]):
            inputs.pop(min_index)
    return output


if __name__ == '__main__':
    input_lists = []
    input_sizes = [5, 4, 3, 6]
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

import heapq
import random
from typing import List


def merge_lists(inputs: List[List[int]]) -> List[int]:
    """
    Merges an arbitrary number of sorted lists of integers
    into a single sorted list of integers.
    Before iteration starts, builds a priority queue (heap) out of
    the initial (smallest) entries in all k lists in the input list of lists,
    with each value paired with the index of its list in the input list of lists.
    On each iteration, the main loop
    - Pops the minimum value and its index i from the heap
    - Pushes that value onto the output list
    - Pops the front value from list i in the input list of lists
    - If list i is not empty, pushes the new front value onto the heap
    - Otherwise (if list i is now empty), notes the loss of one list.
    Iteration finishes when all the lists in the input list are empty.

    :param inputs: list of sorted lists of integers
    :return sorted list of integers
    """
    output = []
    first_items = []
    for index, input_l in enumerate(inputs):
        first_items.append((input_l[0], index))
    heapq.heapify(first_items)
    num_lists = len(inputs)
    while num_lists:
        next_item, min_index = heapq.heappop(first_items)
        output.append(next_item)
        inputs[min_index].pop(0)
        if len(inputs[min_index]):
            heapq.heappush(first_items, (inputs[min_index][0], min_index))
        else:
            num_lists -= 1
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

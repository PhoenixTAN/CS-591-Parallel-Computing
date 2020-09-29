import threading
import random
from typing import List
import math

def merge_two_lists(list1: List[int], list2: List[int]) -> List[int]:
    '''
        sequential version to merge two lists
    '''
    M = len(list1)
    N = len(list2)

    list3 = [0] * (M + N)

    i = 0
    j = 0
    k = 0
    while i < M and j < N:
        if list1[i] < list2[j]:
            list3[k] = list1[i]
            i += 1
        else:
            list3[k] = list2[j]
            j += 1
        k += 1

    while i < M:
        list3[k] = list1[i]
        i += 1
        k += 1

    while j < N:
        list3[k] = list2[j]
        j += 1
        k += 1

    return list3

def validate_result(list1: List[int], list2: List[int]) -> bool:
    '''
        For test purpose, the result from sequential version and that from parallel version should be the same.
    '''
    if len(list1) != len(list2):
        return False
    
    for i in range(len(list1)):
        if list1[i] != list2[i]:
            return False

    return True


def parallel_merge_two_lists(list1: List[int], list2: List[int]) -> List[int]:
    '''
        parallel version to merge two lists.
    '''

    M = len(list1)
    N = len(list2)
    num_of_processors = int((M + N) // math.log(M + N, 2))

    threads = [None] * num_of_processors    # create threads pools
    list3 = [0] * (M + N)   # initialize the final result
    for i in range(len(threads)):
        threads[i] = threading.Thread(target=worker, args=(i, list1, list2, list3))

    for thread in threads:
        thread.start()
    
    for thread in threads:
        thread.join()

    return list3


def worker(thread_id: int, list1: List[int], list2: List[int], list3: List[int]) -> None:
    '''
        Each threads take care log(m+n) numbers
        where m is the length of list1 and n is the length of list2.
        Find the insert position in another list and compute the final index in list3.
        For example:
            list1 = [2, 3, 4, 5]
            list2 = [0, 1, 2, 3]
            Processor_0 computes the the first three numbers in list1.
            For list1[0] which is 2, it should be inserted in the 3rd position in list2.
            Then, list3[0+3] = 2.
    '''
    M = len(list1)
    N = len(list2)
    chunk_size = int(math.log(M + N, 2))

    start_index = thread_id * chunk_size

    for index in range(start_index, start_index + chunk_size):
        if index >= M:
            # target number is in list2
            # target array is list1
            j = index - M
            i = search_insert_position(list1, list2[j], True)
            list3[i+j] = list2[j]
        else:
            # target number is in list1
            # target array is list2
            i = index
            j = search_insert_position(list2, list1[i], False)
            list3[i+j] = list1[i]

    return None


def search_insert_position(arr: List[int], target: int, flag: bool) -> int:
    '''
        A helper function for parallel computing.
        Use binary search to find the insert position in a sorted array.

        param arr: a sorted array
        param target: the number need to be inserted into the list
        param flag: a flag to indicate the position we need to insert

        Let's say more about the flag parameter.
        Example:
            array = [1, 2, 2, 2, 3], target = 2
        If flag is True, we return 1.
        If flag is False, we return 4.

        By doing that, we can avoid the problem where we would miss some numbers in the final results.      
    '''
    left = 0
    right = len(arr) - 1

    while left <= right:
        mid = left + (right - left) // 2

        if flag:
            if target > arr[mid]:
                left = mid + 1
            else:
                right = mid - 1
        else:
            if target >= arr[mid]:
                left = mid + 1
            else:
                right = mid - 1

    return left

if __name__ == '__main__':

    # parameters
    M = 7
    N = 9

    # initialize two lists
    list1 = [0] * M
    list2 = [0] * N
    # make sure they are sorted in ascending order
    for i in range(1, M):
        list1[i] += list1[i-1] + random.randint(1, 5)
    
    for i in range(1, N):
        list2[i] += list2[i-1] + random.randint(1, 5)

    print(list1)
    print(list2)

    # sequential version
    list3_sequential = merge_two_lists(list1, list2)
    print(list3_sequential)

    # parallel version
    list3_parallel = parallel_merge_two_lists(list1, list2)
    print(list3_parallel)

    # validating result
    print(validate_result(list3_sequential, list3_parallel))
    


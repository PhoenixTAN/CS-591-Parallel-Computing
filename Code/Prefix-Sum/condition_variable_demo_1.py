from multiprocessing import Array, Condition, Process
import random

# Demonstration of the use of condition variables in stand-alone functions.


def setter(array: Array, cv: Condition):
    """
    Sets the value of the first (and only) index
    position in array exclusively and notifies the process
    awaiting this action that a valid value is available.
    :param array: a multiprocessing Array of integers of size 1 to which to write a value
    :param cv: a Condition object (a condition variable) to allow the value to be read only when it is ready
    """
    print('In setter.')
    with cv:
        print('Before setting:', array[0])
        array[0] = 43
        print('After setting:', array[0])
        cv.notify()


def getter(array: Array, cv: Condition):
    """
    Awaits notification through a Condition object
    (condition variable) that a value is available in array.
    Reads the value and prints it.
    :param array: a multiprocessing Array of integers of size 1 from which to read a value
    :param cv: a Condition object (a condition variable) to allow the value to be read only when it is ready
    """
    print('In getter.')
    with cv:
        # wait_for takes a _predicate_, which is a boolean-valued function.
        # A lambda expression creates an anonymous function that returns the result of evaluating the body.
        # This invocation causes the process to block until the value in a is set.
        cv.wait_for(lambda: array[0] != 0)
        print(f'Got {array[0]}.')


# Driver function
# Creates a setter process (p0) and a getter process (p1).
# Prints the contents of the shared Array after the setter has written the value
# and the
if __name__ == '__main__':
    array = Array('i', 1)
    cv = Condition()
    print('In main before setting and getting:', array[0])
    p0 = Process(target=setter, args=(array, cv))
    p1 = Process(target=getter, args=(array, cv))
    p1.start()
    p0.start()
    p1.join()
    p0.join()
    print('In main after setting and getting:', array[0])

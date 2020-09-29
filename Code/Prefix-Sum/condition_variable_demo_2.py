from multiprocessing import Array, Condition, Process
import random


class ConditionVariableDemo:
    """
    Demonstrates the use of condition variables within a class.
    """

    def __init__(self):
        self.array = Array('i', 1)
        self.cv = Condition()

    def run(self):
        """
        Creates, runs, and joins a setter and a getter process.
        """
        p0 = Process(target=self.setter)
        p1 = Process(target=self.getter)
        p0.start()
        p1.start()
        p0.join()
        p1.join()

    def setter(self):
        """
        Sets the value of the first (and only) index
        position in a exclusively and notifies the process
        awaiting this action that a valid value is available.
        :param a: a multiprocessing Array of integers of size 1 to which to write a value
        :param cv: a Condition object (a condition variable) to allow the value to be read only when it is ready
        """
        with self.cv:
            print('In setter.')
            print('Before setting:', self.array[0])
            self.array[0] = 43
            print('After setting:', self.array[0])
            self.cv.notify()

    def getter(self):
        """
        Awaits notification through a Condition object
        (condition variable) that a value is available in a.
        Reads the value and prints it.
        :param a: a multiprocessing Array of integers of size 1 from which to read a value
        :param cv: a Condition object (a condition variable) to allow the value to be read only when it is ready
        """
        print('In getter.')
        with self.cv:
            self.cv.wait_for(lambda: self.array[0] != 0)
            print(f'Got {self.array[0]}.')


if __name__ == '__main__':

    c = ConditionVariableDemo()
    print('In main before setting and getting:', c.array[0])
    c.run()
    print('In main after setting and getting:', c.array[0])

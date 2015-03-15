__author__ = 'mbaxpac2'


class Item:
    """ Represents an item that can be part of a solution for a KP instance. """

    def __init__(self, label, value, weight):
        self.label = label
        self.value = value
        self.weight = weight

    def __str__(self):
        return '\t{0}\t{1}\t{2}'.format(self.label, self.value, self.weight)


class KP:
    """ Class for creating instances of KP problems. """

    def __init__(self, filename):
        with open(filename) as f:

            self.n = int(f.readline())

            self.items = []

            for i in range(self.n):
                line = f.readline()

                s = line.split()

                label = int(s[0])
                value = int(s[1])
                weight = int(s[2])

                self.items.append(Item(label, value, weight))

            self.c = int(f.readline())

    def __str__(self):
        s = str(self.n) + '\n'

        for item in self.items:
            s += str(item) + '\n'

        s += str(self.c)

        return s

    def evaluate_solution(self, sol):
        """ Evaluate a solution in binary string format in the instance. """

        solution_value = 0
        solution_weight = 0

        for i in range(len(sol)):
            # Iterate every item if KP instance.
            if sol[i] == '1':
                # Sum values and weights of items that are present in solution.
                solution_value += self.items[i].value
                solution_weight += self.items[i].weight

        if solution_weight <= self.c:
            return {'value': solution_value, 'weight': solution_weight, 'sol': sol}
        else:
            return -1

    def str_solution(self, sol):
        """ Return a string with the labels of the items present in a solution in binary string format. """

        s = ''

        for i in range(len(sol)):
            if sol[i] == '1':
                s += str(self.items[i].label) + ' '

        return s


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

    def __init__(self, filename, opt=None):
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

        if opt == 1:
            self.items.sort(key=lambda item: item.value, reverse=True)
        elif opt == 2:
            self.items.sort(key=lambda item: float(item.value) / item.weight, reverse=True)

        #for item in self.items:
            #print(item)

    def __str__(self):
        s = str(self.n) + '\n'

        for item in self.items:
            s += str(item) + '\n'

        s += str(self.c)

        return s

    def __iter__(self):
        return iter(self.items)

    def evaluate_solution(self, sol, lim=-1):
        """ Evaluate a solution in binary string format in the instance. """

        solution_value = 0
        solution_weight = 0

        if lim == -1:
            lim = len(sol)

        for i in range(lim):
            # Iterate every item if KP instance.
            if sol[i] == '1':
                # Sum values and weights of items that are present in solution.
                solution_value += self.items[i].value
                solution_weight += self.items[i].weight

        if solution_weight <= self.c:
            return solution_value, solution_weight
        else:
            return False

    def frac_upper_bound(self, sol):
        """ Evaluate a solution in binary string format in the instance. """

        ob = 0
        solution_weight = 0
        capacity = self.c

        for i in range(len(sol)):
            # Iterate every item if KP instance.
            if sol[i] == '1':
                current_item = self.items[i]

                if solution_weight + current_item.weight > capacity:
                    allowed_weight = capacity - solution_weight

                    ob += int(allowed_weight * current_item.value / current_item.weight)

                    break

                ob += current_item.value
                solution_weight += current_item.weight

                if solution_weight == capacity:
                    break

        return ob

    def str_solution(self, sol):
        """ Return a string with the labels of the items present in a solution in binary string format. """

        s = ''

        for i in range(len(sol)):
            if sol[i] == '1':
                s += str(self.items[i].label) + ' '

        return s

    def verify_solution(self, sol, value, weight):
        evaluation = self.evaluate_solution(sol)

        if evaluation[0] == value or evaluation[1] == weight:
            return True
        else:
            return False


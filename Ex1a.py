__author__ = 'mbaxpac2'


class Item:
    def __init__(self, label, value, weight):
        self.label = label
        self.value = value
        self.weight = weight

    def __str__(self):
        return '\t{0}\t{1}\t{2}'.format(self.label, self.value, self.weight)

class KP:
    def __init__(self, filename):
        with open(filename) as f:

            self.N = int(f.readline())

            self.items = []

            for i in range(self.N):
                line = f.readline()

                s = line.split()

                label = int(s[0])
                value = int(s[1])
                weight = int(s[2])

                self.items.append(Item(label, value, weight))

            self.C = int(f.readline())

    def __str__(self):
        s = str(self.N) + '\n'

        for item in self.items:
            s += str(item) + '\n'

        s += str(self.C)

        return s

    def evaluate_solution(self, sol):
        solution_value = 0
        solution_weight = 0

        for i in range(len(sol)):
            if sol[i] == '1':
                solution_value += self.items[i].value
                solution_weight += self.items[i].weight

        if solution_weight <= self.C:
            return {'value': solution_value, 'weight': solution_weight, 'sol': sol}
        else:
            return -1

    def str_solution(self, sol):
        s = ''

        for i in range(len(sol)):
            if sol[i] == '1':
                s += str(i) + ' '

        return s

#kp_instance = KP('easy.20.kp.txt')

#print(kp_instance)
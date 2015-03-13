__author__ = 'mbaxpac2'

class AdjMatGraph:
    """ Adjacecy Matrix representation of a Graph """
    def __init__(self, num_vertexes, num_edges):
        self.num_vertexes = num_vertexes
        self.num_edges = num_edges

        adj = []

    def __init__(self, filename):
        f = open(filename)

        line = int(f.readline())

        print(line)

        f.close()
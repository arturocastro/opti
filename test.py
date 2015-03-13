__author__ = 'mbaxpac2'

from queue import PriorityQueue
from AdjMatGraph import AdjMatGraph

import numpy

q = PriorityQueue()

q.put(2)

q.put(1)

q.put(10)

q.put(7)

print(q.get())

print("hola")

g = AdjMatGraph("tinyEGW.txt")



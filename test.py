__author__ = 'mbaxpac2'

from queue import PriorityQueue
from AdjMatGraph import AdjMatGraph
import heapq

import numpy

q = PriorityQueue()

q.put(2)
q.put(1)
q.put(10)
q.put(7)

print(q.get())

print("hola")

#g = AdjMatGraph("tinyEGW.txt")

qq = []

heapq.heappush(qq, 2)
heapq.heappush(qq, 1)
heapq.heappush(qq, 10)
heapq.heappush(qq, 7)

print(heapq.heappop(qq))

print("hola")

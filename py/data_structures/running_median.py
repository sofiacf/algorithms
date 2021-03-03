import math
from collections import deque

from py.data_structures.myheap import MyHeap


class RunningMedian:
    def __init__(self, max_size=math.inf):
        self.min_heap = MyHeap(lambda x: -x)
        self.max_heap = MyHeap(lambda x: x)
        self.max_size = max_size
        self.queue = deque()

    def add(self, value):
        if self.size == self.max_size:
            value_to_remove = self.queue.popleft()
            if value_to_remove >= self.min_heap.peek():
                self.min_heap.remove(value_to_remove)
            else:
                self.max_heap.remove(value_to_remove)

        min_heap_peek = self.min_heap.peek()
        if min_heap_peek is None:
            self.min_heap.push(value)
        elif value >= min_heap_peek:
            self.min_heap.push(value)
        else:
            self.max_heap.push(value)

        self._rebalance_heaps()
        self.queue.append(value)
        return self.median

    @property
    def median(self):
        if self.min_heap.size > self.max_heap.size:
            return self.min_heap.peek()
        if self.max_heap.size > self.min_heap.size:
            return self.max_heap.peek()
        return (self.max_heap.peek() + self.min_heap.peek()) / 2

    @property
    def size(self):
        return self.min_heap.size + self.max_heap.size

    def _rebalance_heaps(self):
        size_diff = self.min_heap.size - self.max_heap.size
        if -2 < size_diff < 2:
            return
        if size_diff == 2:  # the min heap is too big!
            self.max_heap.push(self.min_heap.pop())
        else:
            self.min_heap.push(self.max_heap.pop())

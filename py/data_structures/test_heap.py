import heapq
from unittest import TestCase

from py.data_structures.myheap import MyHeap


class TestHeap(TestCase):
    def test_push(self):
        native_heap = []
        my_heap = MyHeap(lambda x: -x)

        for value in [3, 4, 76, 2, 19, 2345, 24667876543, 345, 6]:
            heapq.heappush(native_heap, value)
            my_heap.push(value)
        self.assertEqual(native_heap, my_heap.data)

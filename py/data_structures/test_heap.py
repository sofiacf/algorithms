from unittest import TestCase

from py.data_structures.heap import Heap

class TestHeap(TestCase):
    def test_push_empty(self):
        self.assertEqual([2], Heap(lambda x, y: x > y).push(2).data)

    def test_push(self):
        expected = [9, 6, 7, 2]
        self.assertEqual(expected, Heap(lambda x, y: x > y).push(2).push(9).push(7).push(6).data)

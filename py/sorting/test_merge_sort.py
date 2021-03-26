import unittest

from py.sorting.merge_sort import MergeSort


class TestMergeSort(unittest.TestCase):
    def test_merge_sort(self):
        def compare_fn(x, y):
            if x > y:
                return 1
            if y > x:
                return -1
            return 0

        with open("../../src/sorting/test_data/sort.txt") as values:
            sorter = MergeSort(compare_fn)
            sorter.sort([int(value) for value in values])
            print('copy operations:', sorter.copy_counter)
            print('comparison operations:', sorter.comparison_counter)
            print('total:', sorter.comparison_counter + sorter.copy_counter)


if __name__ == '__main__':
    unittest.main()

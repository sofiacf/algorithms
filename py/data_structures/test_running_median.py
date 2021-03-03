import unittest
from py.data_structures.running_median import RunningMedian


class TestRunningMedian(unittest.TestCase):
    def test_floats(self):
        total = 0
        running_median = RunningMedian()
        with open("../../src/running_median/test_data/random_floating_points.txt") as values:
            for value in values:
                total += running_median.add(float(value))
        self.assertAlmostEqual(4995738.755804, total, 5)

    def test_100_floats(self):
        total = 0
        running_median = RunningMedian(100)
        with open("../../src/running_median/test_data/random_floating_points.txt") as values:
            for value in values:
                total += running_median.add(float(value))
        self.assertAlmostEqual(4995205.397700, total, 5)

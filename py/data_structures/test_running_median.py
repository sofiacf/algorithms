import unittest
from py.data_structures.running_median import RunningMedian


class TestRunningMedian(unittest.TestCase):
    def test_floats(self):
        sum = 0
        running_median = RunningMedian()
        with open("../../src/running_median/test_data/random_floating_points.txt") as values:
            for value in values:
                sum += running_median.add(float(value))
        self.assertAlmostEqual(sum, 4995738.755804099)  # I appended 099 to the expected value. hope that's cool!

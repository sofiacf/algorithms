import unittest

from py.closest_pair import ClosestPair


class TestClosestPair(unittest.TestCase):
    def test_one_dimensional(self):
        with open("../src/closest_pair/test_data/one_dimensional_points.txt") as values:
            pair_util = ClosestPair([float(value) for value in values])
            print(pair_util.one_dimensional())

    def test_two_dimensional(self):
        with open("../src/closest_pair/test_data/points.txt") as values:
            string_pairs = [value.split(',') for value in values]
        pair_util = ClosestPair([(float(pair[0]), float(pair[1])) for pair in string_pairs])
        pair_util.two_dimensional()
        self.assertAlmostEqual(pair_util.shortest_distance, 0.068070, 6)
        closest_points = pair_util.closest_points

        self.assertEqual(closest_points[0], (38523.977990, -33081.309257))
        self.assertEqual(closest_points[1], (38524.043694, -33081.291468))

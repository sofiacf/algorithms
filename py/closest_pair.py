import math
from typing import Union


class ClosestPair:
    data: Union[list[float], list[tuple[float, float]]]

    def __init__(self, data):
        self.data = data
        self.shortest_distance = math.inf
        self.closest_points = None

    def one_dimensional(self):
        self.data.sort()
        return self._one_dimensional_recursive(self.data)

    def _one_dimensional_recursive(self, data: list[float]):
        length = len(data)
        if length <= 3:
            return self._one_dimensional_brute_force(data)

        mid = length // 2
        left_closest_dist = self._one_dimensional_recursive(data[:mid])
        right_closest_dist = self._one_dimensional_recursive(data[mid:])
        split_closest_dist = self.data[mid] - self.data[mid - 1]

        return min(left_closest_dist, right_closest_dist, split_closest_dist)

    def _one_dimensional_brute_force(self, data: list[float]):
        length = len(data)
        if length == 0 or length == 1:
            print('we split to a length of 0 or 1 —— something is wrong!')
            return None
        if length == 2:
            return data[1] - data[0]
        return min(data[1] - data[0], data[2] - data[1])

    def two_dimensional(self):
        self.data.sort()
        y_sorted_points = self.data.copy()
        y_sorted_points.sort(key=lambda pair: pair[1])
        self._two_dimensional_recursive(self.data, y_sorted_points)
        return self.shortest_distance, self.closest_points

    def _two_dimensional_recursive(self, x_sorted_points, y_sorted_points):
        length = len(x_sorted_points)
        if length <= 3:
            return self._two_dimensional_brute_force(x_sorted_points)
        mid = length // 2
        left_cut = x_sorted_points[:mid]
        right_cut = x_sorted_points[mid:]

        left_cut_y = []
        right_cut_y = []
        x_split = x_sorted_points[mid][0]
        for point in y_sorted_points:
            if point[0] <= x_split:
                left_cut_y.append(point)
            else:
                right_cut_y.append(point)

        left_closest_dist = self._two_dimensional_recursive(left_cut, left_cut_y)
        right_closest_dist = self._two_dimensional_recursive(right_cut, right_cut_y)
        closest_dist = min(left_closest_dist, right_closest_dist)

        # making the strip
        strip = [point for point in y_sorted_points if abs(point[0] - x_split) <= closest_dist]
        for index, point in enumerate(strip):
            i = index + 1
            while i < len(strip) and point[1] - strip[i][1] < closest_dist:
                closest_dist = min(closest_dist, self._distance(point, strip[i]))
                i += 1

        return closest_dist

    def _two_dimensional_brute_force(self, points):
        length = len(points)
        if length == 0 or length == 1:
            raise ValueError('we split to a length of 0 or 1 —— something is wrong!')
        if length == 2:
            return self._distance(points[0], points[1])
        return min([self._distance(points[x], points[y]) for x, y in [(0, 1), (1, 2), (0, 2)]])

    def _distance(self, point1, point2):
        distance = (((point2[0] - point1[0]) ** 2) + ((point2[1] - point1[1]) ** 2)) ** .5
        if distance < self.shortest_distance:
            self.closest_points = point1, point2
            self.shortest_distance = distance
        return distance

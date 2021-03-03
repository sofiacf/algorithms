from collections import deque


class MyHeap:
    def __init__(self, priority):
        self.data = []
        self.priority = priority

    def push(self, item):
        self.data.append(item)
        self._bubble_up(self.last_index)
        return self

    def pop(self):
        self._swap(0, self.last_index)
        return_value = self.data.pop(self.last_index)
        self._bubble_down(0)
        return return_value

    def peek(self):
        if self.last_index == -1:
            return None
        return self.data[0]

    @property
    def last_index(self):
        return len(self.data) - 1

    @property
    def size(self):
        return self.last_index + 1

    def _bubble_up(self, index):
        parent_index = self._get_parent_index(index)
        if parent_index == -1:
            return
        if self.priority(self.data[index]) > self.priority(self.data[parent_index]):
            self._swap(index, parent_index)
            self._bubble_up(parent_index)

    def _bubble_down(self, index):
        left_child_index = self._get_left_child_index(index)
        if left_child_index > self.last_index:
            return
        right_child_index = self._get_right_child_index(index)
        if right_child_index > self.last_index:
            child_index = left_child_index
        else:
            child_index = right_child_index if self.priority(self.data[right_child_index]) >= self.priority(
                self.data[left_child_index]) else left_child_index
        if self.priority(self.data[index]) < self.priority(self.data[child_index]):
            self._swap(index, child_index)
            self._bubble_down(child_index)

    @staticmethod
    def _get_parent_index(index):
        return (index - 1) // 2

    @staticmethod
    def _get_left_child_index(index):
        return (2 * index) + 1

    @staticmethod
    def _get_right_child_index(index):
        return 2 * (index + 1)

    def _swap(self, index, new_index):
        if new_index == -1 or index == -1:
            return
        self.data[index], self.data[new_index] = self.data[new_index], self.data[index]

    def remove(self, value):
        index = self._find(value)
        if index == self.last_index:
            self.data.pop()
            return
        self._swap(index, self.last_index)
        self.data.pop()
        self._bubble_down(index)
        self._bubble_up(index)

    def _find(self, value):
        # return self._recursive_search(value, [0])
        return self._iterative_search(value)

    def _iterative_search(self, value):
        queue = deque()
        queue.append(0)
        while queue:
            index = queue.popleft()
            if self.data[index] == value:
                return index
            priority = self.priority(value)
            left_child_index = self._get_left_child_index(index)
            right_child_index = self._get_right_child_index(index)

            if left_child_index <= self.last_index:
                if self.priority(self.data[left_child_index]) >= priority:
                    queue.append(left_child_index)
            if right_child_index <= self.last_index:
                if self.priority(self.data[right_child_index]) >= priority:
                    queue.append(right_child_index)
        raise Exception()

    def _recursive_search(self, value, indices_to_check):
        new_indices_to_check = []
        for index in indices_to_check:
            if self.data[index] == value:
                return index
            priority = self.priority(value)
            left_child_index = self._get_left_child_index(index)
            right_child_index = self._get_right_child_index(index)

            if left_child_index <= self.last_index:
                if self.priority(self.data[left_child_index]) >= priority:
                    new_indices_to_check.append(left_child_index)
            if right_child_index <= self.last_index:
                if self.priority(self.data[right_child_index]) >= priority:
                    new_indices_to_check.append(right_child_index)
        return self._recursive_search(value, new_indices_to_check)

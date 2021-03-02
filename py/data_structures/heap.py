class Heap:
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
        return self.data[0]

    @property
    def last_index(self):
        return len(self.data) - 1

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
        right_child_index = left_child_index + 1
        if right_child_index > self.last_index:
            child_index = left_child_index
        else:
            child_index = right_child_index if self.priority(self.data[right_child_index]) >= self.priority(self.data[left_child_index]) else left_child_index
        if self.data[index] < self.data[child_index]:
            self._swap(index, child_index)
            self._bubble_down(child_index)

    @staticmethod
    def _get_parent_index(index):
        return (index - 1) // 2

    @staticmethod
    def _get_left_child_index(index):
        return (2 * index) + 1

    def _swap(self, index, new_index):
        if new_index == -1:
            pass
        self.data[index], self.data[new_index] = self.data[new_index], self.data[index]

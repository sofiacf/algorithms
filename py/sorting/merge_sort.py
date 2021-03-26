class MergeSort:
    def __init__(self, compare_fn):
        self.compare_fn = compare_fn
        self.copy_counter = 0
        self.comparison_counter = 0

    def sort(self, data):
        length = len(data)
        if length == 1 or length == 0:
            return data

        first_half = data[:length // 2]
        second_half = data[length // 2:]

        self.copy_counter += 1

        first_half = self.sort(first_half)
        second_half = self.sort(second_half)

        output = []
        first_half_index = 0
        second_half_index = 0

        while True:
            if first_half_index == len(first_half):
                output.extend(second_half[second_half_index:])
                break
            if second_half_index == len(second_half):
                output.extend(first_half[first_half_index:])
                break

            if self.compare_fn(first_half[first_half_index], second_half[second_half_index]) > 0:
                output.append(first_half[first_half_index])
                first_half_index += 1
            else:
                output.append(second_half[second_half_index])
                second_half_index += 1

            self.comparison_counter += 1
            self.copy_counter += 1

        return output

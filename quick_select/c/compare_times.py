import ctypes
import sys
from enum import IntEnum
import statistics
import matplotlib
matplotlib.use('Agg')
from matplotlib import pyplot as plt #noqa

lib = ctypes.CDLL('./algo.so')

NUM_TIME_RUNS = 3
TEST_FOR_Ns = [10 ** 2, 10 ** 3, 10 ** 4, 10 ** 5, 10 ** 6]


class CtypesEnum(IntEnum):
    """A ctypes-compatible IntEnum superclass."""
    @classmethod
    def from_param(cls, obj):
        return int(obj)


class Algo(CtypesEnum):
    QUICK_SELECT = 1,
    SORT_SELECT = 2,


lib.select_time.argtypes = [ctypes.c_size_t, Algo]
lib.select_time.restype = ctypes.c_double


def median_run_time(n, algo):
    times = []

    for i in range(NUM_TIME_RUNS):
        times.append(lib.select_time(n, algo))

    return statistics.median(times)


def generate_md_table(ns, data):
    n_headers = ""
    header_sep = "--|"

    for n in ns:
        n_headers += 'n={:d} |'.format(n)
        header_sep += "--|"

    print("|ALGORITHM|", n_headers)
    print(header_sep)

    for d in data:
        times = ""
        for v in d[1]:
            times += '{:.6f} sec|'.format(v)

        print('|{} |{}'.format(d[0].name, times))

    sys.stdout.flush()


def generate_chart():
    full_data = []
    plt.figure(figsize=(8, 6))
    plt.title('Quick Select vs. Sort Select')
    plt.ylabel('sec')
    plt.xlabel('n')

    for algo in Algo:
        print('running ', algo.name)
        sys.stdout.flush()

        data = []
        for n in TEST_FOR_Ns:
            sys.stdout.flush()

            time = median_run_time(n, algo)
            data.append(time)

        plt.plot(TEST_FOR_Ns, data, label=algo.name)
        full_data.append((algo, data))

    plt.legend()
    plt.savefig('QUICK_SELECT.png')
    plt.clf()

    print('chart created')

    generate_md_table(TEST_FOR_Ns, full_data)


if __name__ == "__main__":
    generate_chart()
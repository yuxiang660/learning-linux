import math
import time

def get_four_square_num(test_num):
    results = []
    for a in range(int(math.sqrt(test_num)) + 1):
        for b in range(a+1):
            for c in range(b+1):
                for d in range(c+1):
                    if (a**2 + b**2 + c**2 + d**2) == test_num:
                        results.append([a, b, c, d])
    return results


if __name__ == '__main__':
    # Takes 158 seconds
    TEST_NUM = 20000
    start = time.time()
    results = get_four_square_num(TEST_NUM)
    print(f"Takes {time.time() - start} seconds")
    print(f"The number of representations of {TEST_NUM} as a sum of four squares with a >= b >= c >= d is {len(results)}.")
    for res in results:
        print(f"The four squares are {res}.")


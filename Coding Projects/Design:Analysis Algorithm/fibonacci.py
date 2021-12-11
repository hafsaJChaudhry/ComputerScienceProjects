import math
import sys
import time

sys.setrecursionlimit(10**6)

def fib(n):
    if n == 1:
        return 0
    elif n == 2:
        return 1
    else:
        return fib(n - 1) + fib(n - 2)

def fib_dp(n, R):
    if R[n - 1] >= 0:
        return R[n - 1]
    else:
        if n == 1:
            result = 0
        elif n == 2:
            result = 1
        else:
            result = fib_dp(n - 1, R) + fib_dp(n - 2, R)
        R[n - 1] = result
        return result
    
def main():
    if len(sys.argv) < 2:
        print('Usage: fib <n>')
    else:
        n = int(sys.argv[1])
        
        start = time.time()
        r = fib(n)
        print('fib({n}) = {r}, took: {duration} seconds'.format(
            n = n,
            r = r,
            duration = time.time() - start))

        start = time.time()
        r = fib_dp(n, [-1] * n)
        print('fib_dp({n}) = {r}, took: {duration} seconds'.format(
            n = n,
            r = r,
            duration = time.time() - start))
        
main()
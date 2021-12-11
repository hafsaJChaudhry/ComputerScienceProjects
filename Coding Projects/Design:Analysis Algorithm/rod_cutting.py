# Price table for rods. Each rod of length i inches earns the company PRICES[i]
# dollars of revenue.
PRICES = {
    1: 1,
    2: 5,
    3: 8,
    4: 9,
    5: 10,
    6: 17,
    7: 17,
    8: 20,
    9: 24,
    10: 30
}

def cut_rod(p, n, R, S):
    if n == 0:
        return 0
    elif R[n - 1] >= 0:
        return R[n - 1]
    else:
        q = -1
        for i in range(1, n + 1):
            r = p[i] + cut_rod(p, n - i, R, S)
            if q < r:
                q = r
                S[n - 1] = i
        R[n - 1] = q   
        return q

def print_solution(p, n, S):
    while n > 0:
        print('cut:', S[n - 1])
        n = n - S[n - 1]
    
def main():
    for i in range(1, 11):
        print('--', i, '--')
        S = [-1] * i
        print('price:', cut_rod(PRICES, i, [-1] * i, S))
        print_solution(PRICES, i, S)

main()
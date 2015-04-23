__author__ = 'mbaxpac2'

from fractions import gcd

divisor = gcd(gcd(gcd(2400000000, 80000), 400000), 8000)

print(divisor)

N_ITEMS = 4
W = 2400000//divisor

w = (80000//divisor, 400000//divisor, 120000//divisor, 8000//divisor)


def xray_v(n):
    if n == 0:
        return 0
    elif n == 1:
        return 1000
    elif n == 2:
        return 1000 + 850
    elif n == 3:
        return 1000 + 850 + 750
    else:
        return False


def nmr_v(n):
    if n == 0:
        return 0
    elif n == 1:
        return 9000
    elif n == 2:
        return 9000 + 2500
    elif n == 3:
        return 9000 + 2500 + 2000
    else:
        return False


def rad_v(n):
    if n == 0:
        return 0
    elif n == 1:
        return 7500
    elif n == 2:
        return 7500 + 4000
    elif n == 3:
        return 7500 + 4000 + 2700
    else:
        return False


def bed_v(n):
    if 0 <= n <= 100:
        return n * 95
    elif n > 100:
        return 9500 + (n - 100) * 80
    else:
        return False


V = N_ITEMS * [None]
keep = N_ITEMS * [None]

for i in range(N_ITEMS):
    V[i] = (W + 1) * [0]
    keep[i] = (W + 1) * [0]

v = (xray_v, nmr_v, rad_v, bed_v)


def bellman(i, curr_w):
    global V
    global v
    global w

    maxi = 0
    k = 0

    for ni in range(curr_w // w[i] + 1):
        if i <= 2 and ni > 3:
            break

        temp = v[i](ni) + V[i - 1][curr_w - ni * w[i]]

        if temp > maxi:
            maxi = temp
            k = ni

    return maxi, k


def dp():
    global V
    global keep

    for i in range(N_ITEMS):
        for curr_w in range(W + 1):
            V[i][curr_w], keep[i][curr_w] = bellman(i, curr_w)


dp()

"""print('\t\t\t'.join(map(str, range(0, (W + 1) * divisor, divisor))))

for item_list in V:
    print('\t\t\t'.join(map(str, item_list)))

print('')

for item_list in keep:
    print('\t\t\t'.join(map(str, item_list)))

print('')"""

K = W

for i in range(N_ITEMS - 1, -1, -1):
    if keep[i][K] > 0:
        ni = keep[i][K]
        print('i={0}\t\tni={1}\t\tpeople={2}\t\tcost={3}'.format(i, ni, v[i](ni), w[i] * ni * divisor))
        K -= w[i] * ni

print('result={0}'.format(V[N_ITEMS - 1][W]))
print('size_i={0} size_w={1} table_size={2}'.format(N_ITEMS, W, N_ITEMS * W))
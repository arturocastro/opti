__author__ = 'mbaxpac2'

N_ITEMS = 4
W = 2400000/8000


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


w = [80000/8000, 400000/8000, 120000/8000, 8000/8000]

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

    for ni in range(curr_w / w[i] + 1):
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
print(V[N_ITEMS - 1][W])

print('\t\t\t'.join(map(str, range(0, (W + 1) * 8000, 8000))))

for item_list in V:
    print('\t\t\t'.join(map(str, item_list)))

print '\n'

for item_list in keep:
    print('\t\t\t'.join(map(str, item_list)))

K = W

for i in range(N_ITEMS - 1, -1, -1):
    if keep[i][K] > 0:
        print i, keep[i][K], 'people={0} cost={1}'.format(v[i](keep[i][K]), w[i] * keep[i][K] * 8000)
        K -= w[i] * keep[i][K]

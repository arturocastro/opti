__author__ = 'mbaxpac2'

HITS_TARGET = 1
AIMS_TARGET_HITS_SINGLE = 2
MISS = 3

BULLSEYE = 21

N_DARTS = 3
SCORES = range(170 + 1)
TARGETS = range(1, 21 + 1) # 21 is BULLSEYE
AIMS = (1, 2, 3)

P = (N_DARTS + 1) * [None]
keep = (N_DARTS + 1) * [None]

for i in range(N_DARTS + 1):
    P[i] = len(SCORES) * [0]
    keep[i] = len(SCORES) * [None]

    # When score is zero, we won so probability is 1
    P[i][0] = 1


def throw_val(aim, target):
    if aim == 1:
        if target == BULLSEYE:
            return 25
        else:
            return target
    elif aim == 2:
        if target == BULLSEYE:
            return 25 * 2
        else:
            return target * 2
    else:
        if target == BULLSEYE:
            # Doesn't exist!
            return 0
        else:
            return target * 3


def throw_p(aim, target, outcome):
    if aim == 1:
        # Any SINGLE or OUTER BULLSEYE
        if outcome == HITS_TARGET:
            return 0.96
        else:
            return 0.04
    elif aim == 2:
        if target == 20:
            # DOUBLE 20
            if outcome == HITS_TARGET:
                return 0.5
            elif outcome == AIMS_TARGET_HITS_SINGLE:
                return 0.35
            else:
                return 0.15
        elif target == BULLSEYE:
            # BULLSEYE
            if outcome == HITS_TARGET:
                return 0.5
            elif outcome == AIMS_TARGET_HITS_SINGLE:
                return 0.4
            else:
                return 0.1
        else:
            # Any DOUBLE
            if outcome == HITS_TARGET or outcome == AIMS_TARGET_HITS_SINGLE:
                return 0.3
            else:
                return 0.4
    else:
        # TREBLE
        if outcome == HITS_TARGET:
            return 0.25
        elif outcome == AIMS_TARGET_HITS_SINGLE:
            return 0.7
        else:
            return 0.05


def bellman(darts, score):
    maxi = 0
    k = None

    for a in AIMS:
        for t in TARGETS:
            if a == 3 and t == BULLSEYE:
                continue

            val = throw_val(a, t)
            delta = score - val

            if delta < 0 or delta == 1:
                # BUST if exceed score or end up with score=1
                continue

            if delta == 0 and a != 2:
                # Can only win with DOUBLE
                continue

            delta_aims_bad = score - val // a

            temp = (throw_p(a, t, HITS_TARGET) * P[darts - 1][delta]) + \
                   (throw_p(a, t, AIMS_TARGET_HITS_SINGLE) * P[darts - 1][delta_aims_bad]) + \
                   (throw_p(a, t, MISS) * P[darts - 1][score])

            if temp > maxi:
                maxi = temp
                k = [a, t]

    return maxi, k


# DP
for d in range(1, N_DARTS + 1):
    for s in range(2, len(SCORES)):
        P[d][s], keep[d][s] = bellman(d, s)

"""
print('\t\t\t'.join(map(str, SCORES)))

for item_list in V:
    print('\t\t\t'.join(map(str, item_list)))

for item_list in keep:
    print('\t\t\t'.join(map(str, item_list)))
"""

for s in range(len(SCORES) - 1, 0, -1):
    K = s

    print('For starting score of {0}...'.format(s))

    for d in range(N_DARTS, -1, -1):
        if keep[d][K]:
            a = keep[d][K][0]
            t = keep[d][K][1]
            optimistic_score = throw_val(keep[d][K][0], keep[d][K][1])

            if t == BULLSEYE:
                if a == 1:
                    t = 'Outer Bullseye'
                elif a == 2:
                    t = 'Bullseye'

            print('darts={0}\tscore={1}\t\taim={2}\ttarget={3}\t\t\t\tP={4}'.format(d, K, a, t, P[d][K]))

            K -= optimistic_score

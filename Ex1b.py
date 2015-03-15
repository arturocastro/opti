__author__ = 'mbaxpac2'


from KP import KP


def enum(kp):
    """ Solve a KP instance by enumeration. """

    best_sol = -1
    best_sol_val = 0

    for subset in range(2 ** kp.n - 1):
        # Enumeration process
        sol = bin(subset)[2:]

        # Fill with zeroes if necessary.
        if len(sol) < kp.n:
            for i in range(kp.n - len(sol)):
                sol = '0' + sol

        evaluation = kp.evaluate_solution(sol)

        if evaluation != -1:
            val = evaluation['value']

            if val > best_sol_val:
                best_sol_val = val
                best_sol = evaluation

    if best_sol_val != -1:
        print('Best feasible solution found: {0} {1}\n{2}'.format(best_sol['value'],
                                                                  best_sol['weight'],
                                                                  kp.str_solution(best_sol['sol'])))


#kp_instance = KP('easy2013.txt')
kp_instance = KP('easy.20.kp.txt')
#kp_instance = KP('my_test.txt')

enum(kp_instance)

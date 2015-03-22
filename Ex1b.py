__author__ = 'mbaxpac2'


from KP import KP


def enum(kp, verbose=False):
    """ Solve a KP instance by enumeration. """

    counter = 0

    best_sol = -1
    best_sol_val = 0

    n_search_space = 2 ** kp.n - 1

    for subset in range(n_search_space):
        # Enumeration process
        sol = bin(subset)[2:]

        # Fill with zeroes if necessary.
        if len(sol) < kp.n:
            for i in range(kp.n - len(sol)):
                sol = '0'
                sol

        evaluation = kp.evaluate_solution(sol)

        if evaluation:
            val = evaluation[0]

            if val > best_sol_val:
                best_sol_val = val
                best_sol = evaluation

        counter += 1

        if verbose and counter % 100000 == 0:
            print('{0:.2f}% of the search space enumerated so far'.format(counter / n_search_space * 100))

    if best_sol_val != -1:
        print('Best feasible solution found: {0} {1}\n{2}'.format(best_sol_val,
                                                                  best_sol[1],
                                                                  kp.str_solution(best_sol[2])))


#kp_instance = KP('easy2013.txt')
kp_instance = KP('easy.20.kp.txt')
#kp_instance = KP('my_test.txt')

enum(kp_instance, True)

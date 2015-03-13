__author__ = 'mbaxpac2'


from Ex1a import KP


def ennumerate(kp):

    best_sol = -1
    best_sol_val = 0

    for subset in range(2 ** kp.N - 1):
        sol = bin(subset)[2:]

        evaluation = kp.evaluate_solution(sol)

        if evaluation == -1:
            continue

        val = evaluation['value']

        if val > best_sol_val:
            best_sol_val = val
            best_sol = evaluation

    return 'Best feasible solution found: {0} {1}\n{2}'.format(best_sol['value'], best_sol['weight'], kp.str_solution(best_sol['sol']))


kp_instance = KP('easy.20.kp.txt')


print(ennumerate(kp_instance))

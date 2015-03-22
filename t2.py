import heapq
from KP import KP


class Solution:
    """ Represents a solution """

    def __init__(self, bin_str, n_specified_items):
        self.bin_str = bin_str
        self.n_specified_items = n_specified_items
        self.bound = 0

    def __lt__(self, other):
        # For max priority queue, return gt instead of lt
        return self.bound > other.bound

    def replace_unassigned_to_zeroes(self):
        solution = list(self.bin_str)

        for i in range(self.n_specified_items, len(self.bin_str)):
            solution[i] = '0'

        return solution

    def __str__(self):
        solution = list(self.bin_str)

        for i in range(self.n_specified_items, len(self.bin_str)):
            solution[i] = '*'

        return str(solution)


kp = KP('easy.20.kp.txt', opt=2)
#kp = KP('easy.200.kp.txt', opt=2)
#kp = KP('hard.200.kp.txt', opt=2)

best_solution_value = 0
best_solution_weight = 0
best_solution = '0' * kp.n

current = Solution(list('1' * kp.n), 0)

current.bound = kp.frac_upper_bound(current.bin_str)

pq = [current]

while len(pq) != 0 and best_solution_value < current.bound:
    current = heapq.heappop(pq)

    new_sol = current.bin_str
    item_to_specify = current.n_specified_items

    if current.n_specified_items != kp.n:
        new_sol[item_to_specify] = '0'
        left = Solution(list(new_sol), current.n_specified_items + 1)

        new_sol[item_to_specify] = '1'
        right = Solution(list(new_sol), current.n_specified_items + 1)

        left.bound = kp.frac_upper_bound(left.bin_str)
        heapq.heappush(pq, left)

        if kp.is_feasible(right.bin_str, right.n_specified_items):
            right.bound = kp.frac_upper_bound(right.bin_str)
            heapq.heappush(pq, right)

            right_solution = right.replace_unassigned_to_zeroes()
            right_solution_evaluation = kp.evaluate_solution(right_solution)

            right_solution_value = right_solution_evaluation[0]

            if right_solution_value > best_solution_value:
                best_solution_value = right_solution_value
                best_solution_weight = right_solution_evaluation[1]
                best_solution = right_solution

print('Best feasible solution found: {0} {1}\n{2}'.format(best_solution_value,
                                                          best_solution_weight,
                                                          kp.str_solution(best_solution)))
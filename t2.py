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

    def __str__(self):
        solution = list(self.bin_str)

        for i in range(self.n_specified_items, len(self.bin_str)):
            solution[i] = '*'

        return ''.join(solution)

# Sort the items (as for greedy)
#kp = KP('easy.20.kp.txt', opt=2)
#kp = KP('easy.200.kp.txt', opt=2)
kp = KP('hard.200.kp.txt', opt=2)

# Best_solution_value = 0
best_solution_value = 0
best_solution_weight = 0
best_solution = ''

# Current = ****...
current = Solution(list('1' * kp.n), 0)

# Current.bound = Fractional_bound (Current)
current.bound = curr_bound = kp.frac_upper_bound(current.bin_str)

# Add_to_priorityQueue (Current)
pq = [current]

while len(pq) != 0 and best_solution_value < curr_bound:
    current = heapq.heappop(pq)
    curr_bound = current.bound

    # if (Current is not a fully-specified solution)
    if current.n_specified_items != kp.n:
        # Replace leftmost *
        item_to_specify = current.n_specified_items

        # Specify one more item for both left and right
        current.n_specified_items += 1

        # Right = Current with leftmost * replaced by 1 --- PART 1
        temp_right_bin_str = current.bin_str[:]
        temp_right_n_specified_items = current.n_specified_items

        # Left = Current with leftmost * replaced by 0
        current.bin_str[item_to_specify] = '0'
        left = current

        # Add_to_PriorityQueue (Left)
        left.bound = kp.frac_upper_bound(left.bin_str)
        heapq.heappush(pq, left)

        # If right is unfeasible, then evaluation is false - Also, Right with all *s replaced by 0s
        right_solution_evaluation = kp.evaluate_solution(temp_right_bin_str, temp_right_n_specified_items)

        # if (Right is feasible)
        if right_solution_evaluation:
            # Right = Current with leftmost * replaced by 1 --- PART 2
            right = Solution(temp_right_bin_str, temp_right_n_specified_items)

            # Add_to_PriorityQueue (Right)
            right.bound = kp.frac_upper_bound(right.bin_str)
            heapq.heappush(pq, right)

            # If (value(Right.solution) > Best_solution_value
            if right_solution_evaluation[0] > best_solution_value:
                # Best_solution = Right.solution
                best_solution = right.bin_str[:right.n_specified_items]

                # Best_solution_value = Right.value(Right.solution)
                best_solution_value = right_solution_evaluation[0]
                best_solution_weight = right_solution_evaluation[1]

                print('val={0} weight={1}\n{2}'.format(best_solution_value, best_solution_weight, right))

if kp.verify_solution(best_solution, best_solution_value, best_solution_weight):
    print('Best feasible solution found: {0} {1}\n{2}'.format(best_solution_value,
                                                              best_solution_weight,
                                                              kp.str_solution(best_solution)))
else:
    print('Something went wrong')


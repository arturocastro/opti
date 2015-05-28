__author__ = 'mbaxpac2'

from KP import KP


#def greedy_approx(kp):

# Create instance of KP problem from file.
#kp = KP('easy.20.kp.txt', opt=2)
kp = KP('easy.200.kp.txt', opt=2)
#kp = KP('hard.200.kp.txt', opt=2)

# Print instance of problem.
#print(kp_instance)

sol_value = 0
sol_weight = 0
sol_item_str = ''

for item in kp:
    if sol_weight + item.weight > kp.c:
        continue

    sol_value += item.value
    sol_weight += item.weight

    sol_item_str += str(item.label) + ' '

if sol_item_str:
    print('Greedy solution (not necessarily optimal): {0} {1}\n{2}'.format(sol_value,
                                                                           sol_weight,
                                                                           sol_item_str))
else:
    print('No solution found, first element overflows capacity: {0}'.format(kp.items[0]))

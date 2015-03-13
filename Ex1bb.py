__author__ = 'mbaxpac2'


from Ex1a import KP


counter = 0


def ennumerate(kp):
    include_list = [False for x in range(kp.N)]

    ennumerate_rec(kp, 0, 19, include_list)



def ennumerate_rec(kp, low, high, include_list):
    if (low >  high):
        #evaluate_solution(include_list)
        print(include_list)

        global counter
        counter += 1

        #'if counter % 1000 == 0:
          #  print(counter)
    else:
        include_list[low] = True
        ennumerate_rec(kp, low + 1, high, include_list)

        include_list[low] = False
        ennumerate_rec(kp, low + 1, high, include_list)


kp_instance = KP('easy.20.kp.txt')

ennumerate(kp_instance)

print(counter)

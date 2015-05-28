#!/bin/bash

sizes=(0.0015 0.001 0.005 0.01 0.05 0.1 0.5 1 5 10)
worsts=(2 5 10 20 30 50)

for s in "${sizes[@]}"
do
    echo "mutation"
    echo $s
    
    for n in "${worsts[@]}"
    do
	echo "worst"
	echo $n

	./ea.sh $s $n
    done
done


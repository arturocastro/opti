#!/bin/bash

# Best:
seed1=7927561
seed2=1862263
seed3=1209739
seed4=3656678
seed5=1896942


if [ -e ea_runs.txt ]; then
    rm ea_runs.txt
fi

echo "EA searches running..."

for (( r=1; r<=5; r++ ))
do 
    < $1 time ./ssea $((seed$r)) 7000 10 3 3 >> ea_runs.txt
    echo "seed="$((seed$r))
done
#echo "done"
#awk '/Fitness/' ea_runs.txt | awk '{print $1,$3}' > ea_fit.txt 
#awk '/Fitness/' ea_runs.txt | awk '{print $1,$5}' > ea_best.txt 

#./mean2 5 6 ea_fit.txt > ea_fit.mean.txt
#./mean2 5 6 ea_best.txt > ea_best.mean.txt

#gnuplot plot_ea.plot

#echo "Plot is in plot_ea.ps" 

#echo "Mean"
#< ea_runs.txt grep fitness | awk '{total += $2} END {print total/NR}' 
< ea_runs.txt grep user | awk '{print $1}' > nums.txt

#R -q -e "x <- read.csv('nums.txt', header = F); summary(x); sd(x[ , 1])"

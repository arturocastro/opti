#!/bin/bash


# CHANGE these seed values 
#seed1=45454
#seed2=6645242
#seed3=656543
#seed4=23232
#seed5=76644
#seed6=87544
#seed7=2098092
#seed8=2209232
#seed9=385223
#seed10=5346653


# New random seeds from random.org
seed1=8364435
seed2=3390711
seed3=5613637
seed4=7488034
seed5=4695072
seed6=5034715
seed7=5034715
seed8=2098004
seed9=5979209
seed10=2948647


if [ -e rs_runs.txt ]; then
    rm rs_runs.txt
fi
 
echo "random searches running..."
for (( r=1; r<=10; r++ ))
do 
    ./rs $((seed$r)) 100000 >> rs_runs.txt 
    echo "seed="$((seed$r))
done
echo "done"
awk '/Fitness/' rs_runs.txt | awk '{print $1,$3}' > rs_fit.txt 
awk '/Fitness/' rs_runs.txt | awk '{print $1,$5}' > rs_best.txt 

./mean2 10 11 rs_fit.txt > rs_fit.mean.txt
./mean2 10 11 rs_best.txt > rs_best.mean.txt

gnuplot plot_rs.plot

echo "============="
awk '/violations=/' rs_runs.txt | awk '{print $2}' > rs_viols.txt
echo "violation statistics"
echo "mean SD SE"
./mean rs_viols.txt
echo "============="

echo "Plot is in plot_rs.ps" 

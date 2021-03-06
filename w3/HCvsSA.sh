#!/bin/bash


# CHANGE these seed values 
#seed1=45454
#seed2=6645242
#seed3=656543
#seed4=23232
#seed5=76644

# New random seeds from random.org
# Best for hc: seed1
# Best for sa: seed3
seed1=2587836
seed2=5489256
seed3=5099546
seed4=3614898
seed5=9633697


if [ -e hc_runs.txt ]; then
    rm hc_runs.txt
fi
 
if [ -e sa_runs.txt ]; then
    rm sa_runs.txt
fi

echo "hc and sa searches running..."

for (( r=1; r<=5; r++ ))
do 
    ./sa $((seed$r)) 500000 0 0 0 >> hc_runs.txt
    ./sa $((seed$r)) 500000 79700 1 500000 >> sa_runs.txt
    echo "seed="$((seed$r))
done
echo "done"
awk '/Fitness/' hc_runs.txt | awk '{print $1,$3}' > hc_fit.txt 
awk '/Fitness/' hc_runs.txt | awk '{print $1,$5}' > hc_best.txt 

awk '/Fitness/' sa_runs.txt | awk '{print $1,$3}' > sa_fit.txt 
awk '/Fitness/' sa_runs.txt | awk '{print $1,$5}' > sa_best.txt 

./mean2 5 51 hc_fit.txt > hc_fit.mean.txt
./mean2 5 51 hc_best.txt > hc_best.mean.txt
./mean2 5 51 sa_fit.txt > sa_fit.mean.txt
./mean2 5 51 sa_best.txt > sa_best.mean.txt

gnuplot plot_sa.plot

echo "====== HC ======"
awk '/violations=/' hc_runs.txt | awk '{print $2}' > hc_viols.txt
echo "violation statistics"
echo "mean SD SE"
./mean hc_viols.txt
echo "============="

echo "====== SA ======"
awk '/violations=/' sa_runs.txt | awk '{print $2}' > sa_viols.txt
echo "violation statistics"
echo "mean SD SE"
./mean sa_viols.txt
echo 
echo "Plot is in plot_sa.ps" 

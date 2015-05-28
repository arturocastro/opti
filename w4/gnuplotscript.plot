set key below
set ylabel "Number of variables set to true"
set xlabel "Number of satisfied clauses out of 160 from one algorithm run"
set xlabel "Number of satisfied clauses out of 160"
set title "Pareto front of test instance aim-200-3_4-yes1-2.cnf from one algorithm run"
plot "nondominated.dat" title "Multiobjective EA: Nondominated Solutions" w points ps 3, "all.dat" title "Multiobjective EA: All Solutions"
set output "moo.gif"
set terminal gif
replot

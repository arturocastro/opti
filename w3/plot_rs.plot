set xlabel "Function Evaluations /1000"
set ylabel "Mean Fitness"
set title "Random Search Results"
set key below
set term post 18 mono
set output "plot_rs.ps"
plot "./rs_best.mean.txt" u ($1/1000):2:4 t "BestSoFar" w err,"./rs_fit.mean.txt" u ($1/1000):2:4 t "Instantaneous" w err,"./rs_best.mean.txt" u ($1/1000):2 t "" w lines lt 1,"./rs_fit.mean.txt" u ($1/1000):2 t "" w lines lt 1
set term gif giant size 1020,760
set output "plot_rs.gif"
replot

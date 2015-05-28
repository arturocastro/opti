set xlabel "Function Evaluations /1000"
set ylabel "Mean Fitness"
set title "Steady State EA Search Results"
set key below
set term post 18
set output "plot_ea.ps"
plot [][80000:100000]"ea_best.mean.txt" u ($1/1000):2:4 t "EA BestSoFar" w err, "ea_fit.mean.txt" u ($1/1000):2:4 t "EA Instantaneous" w err, "./ea_best.mean.txt" u ($1/1000):2 t "" w l lt 1, "ea_fit.mean.txt" u ($1/1000):2 t "" w l lt 2
set term gif giant size 1020,760
set output "plot_ea.gif"
replot

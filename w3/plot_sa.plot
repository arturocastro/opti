set xlabel "Function Evaluations /1000"
set ylabel "Mean Fitness"
set title "HC vs SA Search Results"
set key below
set term post 18
set output "plot_sa.ps"
plot [][0:12000]"hc_best.mean.txt" u ($1/1000):2:4 t "HC BestSoFar" w err, "hc_fit.mean.txt" u ($1/1000):2:4 t "HC Instantaneous" w err, "./hc_best.mean.txt" u ($1/1000):2 t "" w l lt 1, "hc_fit.mean.txt" u ($1/1000):2 t "" w l lt 2, "sa_best.mean.txt" u ($1/1000):2:4 t "SA BestSoFar" w err, "sa_fit.mean.txt" u ($1/1000):2:4 t "SA Instantaneous" w err, "./sa_best.mean.txt" u ($1/1000):2 t "" w l lt 1, "sa_fit.mean.txt" u ($1/1000):2 t "" w l lt 2 
set term gif giant size 1020,760
set output "plot_sa.gif"
replot

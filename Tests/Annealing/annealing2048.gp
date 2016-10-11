set   autoscale
set term epslatex size 7, 5.24 standalone color colortext 10
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically

set xlabel '$g_{scale}$'
set ylabel '$h$'

set title 'Failure rate by $h$ and $g_{scale}$ with $n$ = 2048'
set zlabel 'Failure rate'
set output 'annealing2048percent.tex'

plot 'Annealing2048.res' using 2:3:6 notitle with image

set title 'Comparisons by $h$ and $g_{scale}$ with $n$ = 2048'
set zlabel 'Comparisons'
set output 'annealing2048comparisons.tex'

plot 'Annealing2048.res' using 2:3:5 notitle with image

set title 'Time by $h$ and $g_{scale}$ with $n$ = 2048'
set zlabel 'Time'
set output 'annealing2048time.tex'

plot 'Annealing2048.res' using 2:3:4 notitle with image
set autoscale
set term epslatex size 7, 5.24 standalone color colortext 10
set output 'branch-misses.tex'
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title 'Branch mispredictions'
set xlabel '$n$'
set ylabel '\#branch mispredictions/$n$'
set logscale x
set yrange [0:15]

plot  	'BitonicSortOld.res' using 1:($3/$1) title 'BitonicSort\_nolimit' with linespoints, \
		'BitonicSort.res' using 1:($3/$1) title 'BitonicSort' with linespoints

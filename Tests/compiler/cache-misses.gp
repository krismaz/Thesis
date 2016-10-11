set   autoscale
set term epslatex size 7, 5.24 standalone color colortext 10
set output 'cache-misses.tex'
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title 'Cache misses per comparison'
set xlabel '$n$'
set ylabel '\#cache misses/\#comparisons'
set logscale x
set yrange[0:0.5]

plot  	'BitonicSortOld.res' using 1:($6/$5) title 'BitonicSort\_nolimit' with linespoints, \
		'BitonicSort.res' using 1:($6/$5) title 'BitonicSort' with linespoints

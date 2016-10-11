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

plot  	'RShellSortPure.res' using 1:($6/$5) title 'RandShell' with linespoints, \
		'AnnealingSortPure.res' using 1:($6/$5) title 'AnnealingSort' with linespoints, \
		'BitonicSort.res' using 1:($6/$5) title 'BitonicSort' with linespoints, \
		'OddEvenMergeSort.res' using 1:($6/$5) title 'OEMergeSort' with linespoints, \
		'STDSort.res' using 1:($6/$5) title '\texttt{std::sort}' with linespoints

set autoscale
set term epslatex size 7, 5.24 standalone color colortext 10
set output 'instructionscomparison.tex'
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title 'Instructions per comparison'
set xlabel '$n$'
set ylabel '\#instructions/\#comparisons'
set logscale x
set yrange[0:100]

plot  	'RShellSortPure.res' using 1:($4/$5) title 'RandShell' with linespoints, \
		'ShakerSort.res' using 1:($4/$5) title 'ShakerSort' with linespoints, \
		'BitonicSort.res' using 1:($4/$5) title 'BitonicSort' with linespoints, \
		'Pratt.res' using 1:($4/$5) title 'Pratt' with linespoints, \
		'STDSort.res' using 1:($4/$5) title '\texttt{std::sort}' with linespoints
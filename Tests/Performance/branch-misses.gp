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

plot  	'RShellSortPure.res' using 1:($3/$1) title 'RandShell' with linespoints, \
		'AnnealingSortPure.res' using 1:($3/$1) title 'AnnealingSort' with linespoints, \
		'BitonicSort.res' using 1:($3/$1) title 'BitonicSort' with linespoints, \
		'OddEvenMergeSort.res' using 1:($3/$1) title 'OEMergeSort' with linespoints, \
		'STDSort.res' using 1:($3/$1) title '\texttt{std::sort}' with linespoints

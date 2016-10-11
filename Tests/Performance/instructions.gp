set   autoscale
set term epslatex size 7, 5.24 standalone color colortext 10
set output 'nlogninstructions.tex'
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title 'Instructions are O($n \log n$)'
set xlabel '$n$'
set ylabel '\#instructions/$n \log n$'
set logscale x

plot  	'RShellSortPure.res' using 1:($4/($1*log($1)/log(2))) title 'RandShell' with linespoints, \
		'AnnealingSortPure.res' using 1:($4/($1*log($1)/log(2))) title 'AnnealingSort' with linespoints, \
		'BitonicSort.res' using 1:($4/($1*log($1)/log(2))) title 'BitonicSort' with linespoints, \
		'OddEvenMergeSort.res' using 1:($4/($1*log($1)/log(2))) title 'OEMergeSort' with linespoints, \
		'STDSort.res' using 1:($4/($1*log($1)/log(2))) title '\texttt{std::sort}' with linespoints

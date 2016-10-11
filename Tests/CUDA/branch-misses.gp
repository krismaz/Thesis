set autoscale
set key out
set key right top
set term epslatex size 7, 5.24 standalone color colortext 10
set output 'branch-misses.tex'
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title 'Branch mispredictions'
set xlabel '$n$'
set ylabel '\#branch misprediction/$n$'
set logscale x
set yrange [0:10]


plot  	'RShellSortPure.res' using 1:($3/$1) title 'RandShell' with linespoints, \
		'BitonicSort.res' using 1:($3/$1) title 'BitonicSort' with linespoints, \
		'OddEvenMergeSort.res' using 1:($3/$1) title 'OEMergeSort' with linespoints, \
		'Pratt.res' using 1:($3/$1) title 'Pratt' with linespoints, \
		'ShakerSort.res' using 1:($3/$1) title 'ShakerSort' with linespoints, \
		'ShakerSortCUDA.res' using 1:($3/$1) title 'ShakerSort\_CUDA' with linespoints linecolor 0, \
		'PrattCUDA.res' using 1:($3/$1) title 'Pratt\_CUDA' with linespoints, \
		'RShellSortPureCUDA.res' using 1:($3/$1) title 'RandShell\_CUDA' with linespoints, \
		'BitonicSortCUDA.res' using 1:($3/$1) title 'BitonicSort\_CUDA' with linespoints, \
		'OddEvenMergeSortCUDA.res' using 1:($3/$1) title 'OEMergeSort\_CUDA' with linespoints
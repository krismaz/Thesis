set   autoscale
set key out
set key right top
set term epslatex size 7, 5.24 standalone color colortext 10
set output 'cache-misses.tex'
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title 'Cache misses'
set xlabel '$n$'
set ylabel '\#cache misses/$n$'
set logscale x

plot  	'RShellSortPure.res' using 1:($6/$1) title 'RandShell' with linespoints, \
		'BitonicSort.res' using 1:($6/$1) title 'BitonicSort' with linespoints, \
		'OddEvenMergeSort.res' using 1:($6/$1) title 'OEMergeSort' with linespoints, \
		'Pratt.res' using 1:($6/$1) title 'Pratt' with linespoints, \
		'ShakerSort.res' using 1:($6/$1) title 'ShakerSort' with linespoints, \
		'ShakerSortSIMD.res' using 1:($6/$1) title 'ShakerSort\_SIMD' with linespoints linecolor 0, \
		'PrattSIMD.res' using 1:($6/$1) title 'Pratt\_SIMD' with linespoints, \
		'RShellSortPureSIMD.res' using 1:($6/$1) title 'RandShell\_SIMD' with linespoints, \
		'BitonicSortSIMD.res' using 1:($6/$1) title 'BitonicSort\_SIMD' with linespoints, \
		'OddEvenMergeSortSIMD.res' using 1:($6/$1) title 'OEMergeSort\_SIMD' with linespoints
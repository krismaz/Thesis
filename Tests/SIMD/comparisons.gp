set   autoscale
set term epslatex size 7, 5.24 standalone color colortext 10
set key out vert
set key right top
set output 'nlogncomparisons.tex'
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title 'Comparisons are $O(n \log n$)'
set xlabel '$n$'
set ylabel '\#comparisons/$n \log n$'
set logscale x
set yrange[0:15]

plot  	'RShellSortPure.res' using 1:($5/($1*log($1)/log(2))) title 'RandShell' with linespoints, \
		'BitonicSort.res' using 1:($5/($1*log($1)/log(2))) title 'BitonicSort' with linespoints, \
		'OddEvenMergeSort.res' using 1:($5/($1*log($1)/log(2))) title 'OEMergeSort' with linespoints, \
		'Pratt.res' using 1:($5/($1*log($1)/log(2))) title 'Pratt' with linespoints, \
		'ShakerSort.res' using 1:($5/($1*log($1)/log(2))) title 'ShakerSort' with linespoints, \
		'ShakerSortSIMD.res' using 1:($5/($1*log($1)/log(2))) title 'ShakerSort\_SIMD' with linespoints linecolor 0, \
		'PrattSIMD.res' using 1:($5/($1*log($1)/log(2))) title 'Pratt\_SIMD' with linespoints, \
		'RShellSortPureSIMD.res' using 1:($5/($1*log($1)/log(2))) title 'RandShell\_SIMD' with linespoints, \
		'BitonicSortSIMD.res' using 1:($5/($1*log($1)/log(2))) title 'BitonicSort\_SIMD' with linespoints, \
		'OddEvenMergeSortSIMD.res' using 1:($5/($1*log($1)/log(2))) title 'OEMergeSort\_SIMD' with linespoints
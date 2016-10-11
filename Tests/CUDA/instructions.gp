set   autoscale
set term epslatex size 7, 5.24 standalone color colortext 10
set key out vert
set key right top
set output 'nlogninstructions.tex'
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title 'Instructions are $O(n \log n$)'
set xlabel '$n$'
set ylabel '\#instructions/$n \log n$'
set logscale x
set xrange[16000:17000000]
set yrange[0:300]

plot  	'RShellSortPure.res' using 1:($4/($1*log($1)/log(2))) title 'RandShell' with linespoints, \
		'BitonicSort.res' using 1:($4/($1*log($1)/log(2))) title 'BitonicSort' with linespoints, \
		'OddEvenMergeSort.res' using 1:($4/($1*log($1)/log(2))) title 'OEMergeSort' with linespoints, \
		'Pratt.res' using 1:($4/($1*log($1)/log(2))) title 'Pratt' with linespoints, \
		'ShakerSort.res' using 1:($4/($1*log($1)/log(2))) title 'ShakerSort' with linespoints, \
		'ShakerSortCUDA.res' using 1:($4/($1*log($1)/log(2))) title 'ShakerSort\_CUDA' with linespoints linecolor 0, \
		'PrattCUDA.res' using 1:($4/($1*log($1)/log(2))) title 'Pratt\_CUDA' with linespoints, \
		'RShellSortPureCUDA.res' using 1:($4/($1*log($1)/log(2))) title 'RandShell\_CUDA' with linespoints, \
		'BitonicSortCUDA.res' using 1:($4/($1*log($1)/log(2))) title 'BitonicSort\_CUDA' with linespoints, \
		'OddEvenMergeSortCUDA.res' using 1:($4/($1*log($1)/log(2))) title 'OEMergeSort\_CUDA' with linespoints
set   autoscale
set term epslatex size 7, 5.24 standalone color colortext 10
set output 'branchdiff.tex'
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title 'Extra Branch Misses / Comparison'
set xlabel '$n$'
set ylabel '\%'

set logscale x

plot 	'< join RShellSortPure.res RShellSortPureSwap.res' using 1:(($4-$2)/$3*100) title 'RandShell' with linespoints, \
		'< join BitonicSort.res BitonicSortSwap.res' using 1:(($4-$2)/$3*100)  title 'BitonicSort' with linespoints, \
		'< join OddEvenMergeSort.res OddEvenMergeSortSwap.res' using 1:(($4-$2)/$3*100)  title 'OEMergeSort' with linespoints, \
		'< join Pratt.res PrattSwap.res' using 1:(($4-$2)/$3*100)  title 'Pratt' with linespoints, \
		'< join AnnealingSortPure.res AnnealingSortPureSwap.res' using 1:(($4-$2)/$3*100)  title 'AnnealingSort' with linespoints, \
		'< join ShakerSort.res OddEvenMergeSortSwap.res' using 1:(($4-$2)/$3*100)  title 'ShakerSort' with linespoints linecolor 0

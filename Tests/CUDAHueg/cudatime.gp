set   autoscale
set term epslatex size 7, 5.24 standalone color colortext 10
set key out vert
set key right top
set output 'cudatime.tex'
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title 'Time is $O(n \log n)$'
set xlabel '$n$'
set ylabel 'time(ms)/$n \log n$'
set logscale x
set yrange[0:0.000015]
set xrange[100000:17000000]


plot		'STDSort.res' using 1:($2/($1*log($1)/log(2))) title 'std::sort' with linespoints linecolor 0, \
		'ShakerSortCUDA.res' using 1:($2/($1*log($1)/log(2))) title 'ShakerSort\_CUDA' with linespoints, \
		'PrattCUDA.res' using 1:($2/($1*log($1)/log(2))) title 'Pratt\_CUDA' with linespoints, \
		'BitonicSortCUDA.res' using 1:($2/($1*log($1)/log(2))) title 'BitonicSort\_CUDA' with linespoints, \
		'OddEvenMergeSortCUDA.res' using 1:($2/($1*log($1)/log(2))) title 'OEMergeSort\_CUDA' with linespoints

set   autoscale
set term epslatex size 7, 5.24 standalone color colortext 10
set output 'LLC.tex'
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title 'Cache misses'
set xlabel '$n$'
set ylabel '\#cache-misses/\#comparisons'
set logscale x

plot  	'OddEvenMergeSort.res' using 1:($5/$6) title 'OEMergeSort' with linespoints, \
		'OddEvenMergeSortBuffer.res' using 1:($5/$6) title 'OEMergeSort\_DBuffer' with linespoints, \
		'OddEvenMergeSortSingle.res' using 1:($5/$6) title 'OEMergeSort\_SBuffer' with linespoints, \
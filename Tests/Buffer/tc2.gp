set   autoscale
set term epslatex size 7, 5.24 standalone color colortext 10
set output 'tc2.tex'
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title 'Time affected by L1 misses'
set xlabel '$n$'
set ylabel 'time(ms)/$n \log^2 n$'
set y2label '\#L1-misses/\#comparisons'
set logscale x
set yrange[0:0.00001]
set xrange[16000:17000000]
set ytics nomirror
set y2tics


plot 	'OddEvenMergeSort.res' using 1:($2/($1*(log($1)/log(2))*(log($1)/log(2)))) title 'Time:OEMergeSort' with linespoints axes x1y1, \
		'OddEvenMergeSortBuffer.res' using 1:($2/($1*(log($1)/log(2))*(log($1)/log(2)))) title 'Time:OEMergeSort\_DBuffer' with linespoints axes x1y1, \
		'OddEvenMergeSortSingle.res' using 1:($2/($1*(log($1)/log(2))*(log($1)/log(2)))) title 'Time:OEMergeSort\_SBuffer' with linespoints axes x1y1, \
		'OddEvenMergeSort.res' using 1:($4/$6) title 'CacheMisses:OEMergeSort' with linespoints axes x1y2, \
		'OddEvenMergeSortBuffer.res' using 1:($4/$6) title 'CacheMisses:OEMergeSort\_DBuffer' with linespoints axes x1y2, \
		'OddEvenMergeSortSingle.res' using 1:($4/$6) title 'CacheMisses:OEMergeSort\_SBuffer' with linespoints axes x1y2 linecolor 0


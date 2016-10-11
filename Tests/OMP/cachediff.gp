print "CACHE"

set autoscale
set term epslatex size 7, 5.24 standalone color colortext 10
set key out vert
set key right top
set output 'cachediff.tex'
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title 'OMP \#L1 cache misses / Sequential \#L1 cache cisses'
set xlabel '$n$'
set ylabel 'Change'
set yrange[0:5]
set xrange[16000:17000000]
unset logscale x

stats '< join RShellSortPure.res RShellSortPureOMP.res' using ($12/$6) every ::5 name 'A'
stats '< join BitonicSort.res BitonicSortOMP.res' using ($12/$6) every ::5 name 'B'
stats '< join OddEvenMergeSort.res OddEvenMergeSortOMP.res' using ($6/$7) every ::5 name 'C'
stats '< join ShakerSort.res ShakerSortOMP.res' using ($12/$6) every ::5 name 'D'
stats '< join Pratt.res PrattOMP.res' using ($12/$6) every ::5 name 'E'

set logscale x

plot  	'< join RShellSortPure.res RShellSortPureOMP.res' using 1:($12/$6) title 'RandShell' with linespoints linecolor 1 linetype 1 linewidth 2, \
		'< join BitonicSort.res BitonicSortOMP.res' using 1:($12/$6) title 'BitonicSort' with linespoints linecolor 2 linetype 2 linewidth 2, \
		'< join OddEvenMergeSort.res OddEvenMergeSortOMP.res' using 1:($12/$6) title 'OEMergeSort' with linespoints linecolor 3 linetype 3 linewidth 2, \
		'< join ShakerSort.res ShakerSortOMP.res' using 1:($12/$6) title 'ShakerSort' with linespoints linecolor 4 linetype 4 linewidth 2, \
		'< join Pratt.res PrattOMP.res' using 1:($12/$6) title 'Pratt' with linespoints linecolor 5 linetype 5 linewidth 2

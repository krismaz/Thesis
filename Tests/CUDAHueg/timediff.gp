set autoscale
set term epslatex size 7, 5.24 standalone color colortext 10
set key out vert
set key right top
set output 'timediff.tex'
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title 'Sequential time / CUDA time'
set xlabel '$n$'
set ylabel 'speedup'
set xrange[16000:17000000]
unset logscale x

stats '< join RShellSortPure.res RShellSortPureCUDA.res' using ($2/$7) every ::5 name 'A'
stats '< join BitonicSort.res BitonicSortCUDA.res' using ($2/$7) every ::5 name 'B'
stats '< join OddEvenMergeSort.res OddEvenMergeSortCUDA.res' using ($2/$7) every ::5 name 'C'
stats '< join ShakerSort.res ShakerSortCUDA.res' using ($2/$7) every ::5 name 'D'
stats '< join Pratt.res PrattCUDA.res' using ($2/$7) every ::5 name 'E'

set logscale x

plot  	'< join RShellSortPure.res RShellSortPureCUDA.res' using 1:($2/$7) title 'RandShell' with linespoints linecolor 1 linetype 1 linewidth 2, \
		'< join BitonicSort.res BitonicSortCUDA.res' using 1:($2/$7) title 'BitonicSort' with linespoints linecolor 2 linetype 2 linewidth 2, \
		'< join OddEvenMergeSort.res OddEvenMergeSortCUDA.res' using 1:($2/$7) title 'OEMergeSort' with linespoints linecolor 3 linetype 3 linewidth 2, \
		'< join ShakerSort.res ShakerSortCUDA.res' using 1:($2/$7) title 'ShakerSort' with linespoints linecolor 4 linetype 4 linewidth 2, \
		'< join Pratt.res PrattCUDA.res' using 1:($2/$7) title 'Pratt' with linespoints linecolor 5 linetype 5 linewidth 2


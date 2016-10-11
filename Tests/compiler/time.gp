set   autoscale
set term epslatex size 7, 5.24 standalone color colortext 10
set output 'nlogntime.tex'
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title 'Time is $O(n \log^2 n)$'
set xlabel '$n$'
set ylabel 'time(ms)/$n \log^2 n$'
set logscale x
set yrange [0:0.000003]

plot  	'BitonicSortOld.res' using 1:($2/($1*(log($1)/log(2))*(log($1)/log(2)))) title 'BitonicSort\_nolimit' with linespoints, \
		'BitonicSort.res' using 1:($2/($1*(log($1)/log(2))*(log($1)/log(2)))) title 'BitonicSort' with linespoints

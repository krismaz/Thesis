import subprocess
import re
from math import sqrt


exe = '../../executables/randshellsort'
sizes = []
s = 1024
while s <= 1024*1024*16:
	sizes.append(s)
	s = int(s*2)
algorithms = ['None','BitonicSort','BitonicSortOld',]
counters = ['time', 'branch-misses', 'instructions', 'comparisons', 'cache-misses']
reges = {'time':re.compile('(.*),task-clock.*'), 'branch-misses':re.compile('(.*),branch-misses.*'), 'instructions':re.compile('(.*),instructions.*'), 'comparisons':re.compile('(.*),comparisons.*'), 'cache-misses':re.compile('(.*),cache-misses.*') }

files = {alg: open(alg + '.res', 'w') for alg in algorithms}

for alg in algorithms:
	files[alg].write('#n\t' + "\t".join(counters) + '\n')

for size in sizes:
	filename = str(size) + 'data.dat'
	subprocess.call(['dd', 'if=/dev/urandom', 'bs='+str(size), 'count=4' , 'of=' + filename])
	for alg in algorithms:
		proc = subprocess.Popen(['perf', 'stat', '-r', '10', '-e', 'task-clock,branch-misses,instructions,cache-misses', '-x,', exe, 'Run',  alg, filename], stdout = subprocess.PIPE, stderr = subprocess.PIPE, universal_newlines = True)
		proc.wait()
		out, err = proc.communicate()
		line = str(size)
		#print(str(err))
		for counter in counters:
			 line = line + '\t' + reges[counter].search(str(err)).group(1)
		files[alg].write(line + '\n')
		files[alg].flush()
		

subprocess.call("rm *.dat", shell = True)
subprocess.call("gnuplot *.gp 2> stats.res", shell = True)
subprocess.call("find -name '*.tex' -exec pdflatex {} \;", shell = True)


print("ALL DONES!")
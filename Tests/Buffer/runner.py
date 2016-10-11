import subprocess
import re
from math import sqrt


exe = '../../executables/randshellsortNOBUFFER'
bufferexe = '../../executables/randshellsort'
singleexe = '../../executables/randshellsortSINGLELAYER'
sizes = []
s = 1024
while s <= 1024*1024*16:
	sizes.append(s)
	s = int(s*2)
algorithms = ['None','OddEvenMergeSort']
counters = ['time', 'cache-misses', 'L1Misses', 'LLCMisses', 'comparisons', 'instructions']
reges = {'time':re.compile('(.*),task-clock.*'), 'cache-misses':re.compile('(.*),cache-misses.*'), 'L1Misses':re.compile('(.*),L1-dcache-load-misses'), 'LLCMisses':re.compile('(.*),LLC-load-misses'), 'comparisons':re.compile('(.*),comparisons.*'), 'instructions':re.compile('(.*),instructions.*') }

files = {alg: open(alg + '.res', 'w') for alg in algorithms}
bufferfiles = {alg: open(alg + 'Buffer.res', 'w') for alg in algorithms}
singlefiles = {alg: open(alg + 'Single.res', 'w') for alg in algorithms}

for size in sizes:
	filename = str(size) + 'data.dat'
	subprocess.call(['dd', 'if=/dev/urandom', 'bs='+str(size), 'count=4' , 'of=' + filename])
	for alg in algorithms:
		proc = subprocess.Popen(['perf', 'stat', '-r', '10', '-e', 'task-clock,cache-misses,L1-dcache-load-misses,LLC-load-misses,instructions', '-x,', exe, 'Run',  alg, filename], stdout = subprocess.PIPE, stderr = subprocess.PIPE, universal_newlines = True)
		proc.wait()
		out, err = proc.communicate()
		line = str(size)
		for counter in counters:
			line = line + '\t' + reges[counter].search(str(err)).group(1)
		files[alg].write(line + '\n')
		files[alg].flush()

		proc = subprocess.Popen(['perf', 'stat', '-r', '10', '-e', 'task-clock,cache-misses,L1-dcache-load-misses,LLC-load-misses,instructions', '-x,', bufferexe, 'Run',  alg, filename], stdout = subprocess.PIPE, stderr = subprocess.PIPE, universal_newlines = True)
		proc.wait()
		out, err = proc.communicate()
		line = str(size)
		for counter in counters:
			 line = line + '\t' + reges[counter].search(str(err)).group(1)
		bufferfiles[alg].write(line + '\n')
		bufferfiles[alg].flush()

		proc = subprocess.Popen(['perf', 'stat', '-r', '10', '-e', 'task-clock,cache-misses,L1-dcache-load-misses,LLC-load-misses,instructions', '-x,', singleexe, 'Run',  alg, filename], stdout = subprocess.PIPE, stderr = subprocess.PIPE, universal_newlines = True)
		proc.wait()
		out, err = proc.communicate()
		line = str(size)
		for counter in counters:
			 line = line + '\t' + reges[counter].search(str(err)).group(1)
		singlefiles[alg].write(line + '\n')
		singlefiles[alg].flush()
		

subprocess.call("rm *.dat", shell = True)
subprocess.call("gnuplot *.gp 2> stats.res", shell = True)
subprocess.call("find -name '*.tex' -exec pdflatex {} \;", shell = True)

print("ALL DONES!")
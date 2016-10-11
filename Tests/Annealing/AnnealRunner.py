import subprocess
import re
from math import sqrt


exe = '../../executables/randshellsort'
sizes = [1024*(2**i) for i in range(11)]
hs = [1]
scales = [0]
counters = ['time', 'comparisons',  'errors']
reges = {'time':re.compile('(.*),task-clock.*'), 'comparisons':re.compile('(.*),comparisons.*'), 'errors':re.compile('(.*),errors.*') }

ofile = open('Annealing256b.res', 'w')

ofile.write('#n\tscale\th\t' + "\t".join(counters) + '\n')

for size in sizes:
	for h in hs:
		for scale in scales:
			proc = subprocess.Popen(['perf', 'stat','-e', 'task-clock', '-x,', exe, 'AnnealingTest', str(size), str(scale), str(h)], stdout = subprocess.PIPE, stderr = subprocess.PIPE, universal_newlines = True)
			proc.wait()
			out, err = proc.communicate()
			line = '\t'.join([str(size), str(scale), str(h)])
			#print(str(err))
			for counter in counters:
				line = line + '\t' + reges[counter].search(str(err)).group(1)
			ofile.write(line + '\n')
			ofile.flush()


subprocess.call("rm *.dat", shell = True)
subprocess.call("gnuplot *.gp", shell = True)
print("ALL DONES!")
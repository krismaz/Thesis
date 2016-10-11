import subprocess
import re
from math import sqrt

size = 4096
filename = 'data.dat'
exe = './randshellsort'
py = 'SIMDerize.py'

algorithms = ['RShellSortPure','OddEvenMergeSort' , 'BitonicSort','OddEvenMergeSortCUDA', 'BitonicSortCUDA', 'Pratt', 'ShakerSort', 'AnnealingSortPure']
stats = dict([(alg, [alg]) for alg in algorithms])

subprocess.call(['dd', 'if=/dev/urandom', 'bs='+str(size), 'count=4' , 'of=' + filename])

for alg in algorithms:
	subprocess.call(exe + ' Run ' + alg + ' ' + filename +  ' > ' + alg, shell = True)
	proc = subprocess.Popen(['wc','-l', alg], stdout = subprocess.PIPE, stderr = subprocess.PIPE, universal_newlines = True)
	proc.wait()
	out, err = proc.communicate()
	stats[alg].append(out.split(' ')[0])
	subprocess.call('pypy3 ' + py + ' -layer True -fname ' + alg +' > ' + alg + '4AA', shell = True)
	proc = subprocess.Popen(['wc','-l', alg+'4AA'], stdout = subprocess.PIPE, stderr = subprocess.PIPE, universal_newlines = True)
	proc.wait()
	out, err = proc.communicate()
	stats[alg].append(out.split(' ')[0])
	subprocess.call('pypy3 ' + py + ' -layer True -UU True -fname ' + alg +' > ' + alg + '4UU', shell = True)
	proc = subprocess.Popen(['wc','-l', alg+'4UU'], stdout = subprocess.PIPE, stderr = subprocess.PIPE, universal_newlines = True)
	proc.wait()
	out, err = proc.communicate()
	stats[alg].append(out.split(' ')[0])
	subprocess.call('pypy3 ' + py + ' -layer True -UU True -shuffle True -fname ' + alg +' > ' + alg + '4SHUF', shell = True)
	proc = subprocess.Popen(['wc','-l', alg+'4SHUF'], stdout = subprocess.PIPE, stderr = subprocess.PIPE, universal_newlines = True)
	proc.wait()
	out, err = proc.communicate()
	stats[alg].append(out.split(' ')[0])

print('\\begin{table}[!ht]')
print('\\begin{tabular}{|l|c|c|c|c|}')
print('Algorithm & Sequential & Aligned & Unaligned & Shuffled \\\\')
print('\\hline')
for alg in algorithms:
	print(' & '.join(stats[alg]) + '\\\\')
	print('\\hline')
print('\\end{tabular}')
print('\\end{table}')
print("ALL DONES!")

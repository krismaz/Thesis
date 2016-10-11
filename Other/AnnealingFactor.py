from math import log2

Ns = '$n$'
Rs = '$\\frac{\log n}{\log \log n}$'

for i in range(10, 25):
	N = 2**i
	Ns = Ns + ', ' + '$2^{}$'.format(i)
	Rs = Rs + ', ' + '{:1.2f}'.format(log2(N)/log2(log2(N)))

print(Ns)
print(Rs)
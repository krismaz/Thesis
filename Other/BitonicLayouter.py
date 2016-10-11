from itertools import product

def recusionGuard(n):
	return ['template<> void BitonicMerge2 < {}, {} > (int * const Array, unsigned int const lo) {}'.format(tf, i, '{}') for tf, i in product(['true', 'false'], range(-n,0))]

def compses(start, off, depth, asc, count = 0):
	up = 'lo + i{}'.format(start)
	down = 'lo + i{} + {}'.format(start, off)
	if not asc:
		up, down = down, up
	result = ['\t\t\tCompareAndExchange2way(Array, {} , {} );'.format(up, down)]
	if depth != 0:
		bla = 'm/{}'.format(2**(count+1))
		result += compses(start, bla, depth-1, asc,  count + 1)
		result += compses(start + ' + ' + off, bla, depth-1, asc, count + 1)
	return result

def recursives(n):
	result = []
	for i in range(2**(n+1)):
			off = ''
			for j in reversed(range(n+1)):
				if i & 2**(j) > 0:
					off += ' + m/{}'.format(2**(n-j))
			result.append('\tBitonicMerge2<Ascending, bits - {} > (Array, lo{});'.format(n+1, off))
	return result

def layout(n):
	result = []
	result.append('if(bits>{}){{'.format(n))
	result.append('\tif (Ascending) {')
	for i in range(n):
		result.append('\t\tfor(int i = m/{}; i < m/{}; ++i){{'.format(2**(i+1), 2**i))
		result += compses('','m', i, True)
		result.append('\t\t}')
	result.append('\t\tfor(int i = 0; i < m/{}; ++i){{'.format(2**n))
	result += compses('','m', n, True)
	result.append('\t\t}')
	result.append('\t} else {')
	for i in range(n):
		result.append('\t\tfor(int i = m/{}; i < m/{}; ++i){{'.format(2**(i+1), 2**i))
		result += compses('','m', i, False)
		result.append('\t\t}')
	result.append('\t\tfor(int i = 0; i < m/{}; ++i){{'.format(2**n))
	result += compses('','m', n, False)
	result.append('\t}')
	result +=  recursives(n)
	result.append('\treturn;')
	result.append('\t}')
	return result

print('\n'.join(layout(4)))
from Networks import BitonicSort,OddEvenMergesort, PeriodicSort, RandShellSort, FourWaySort
from Layerize import layerize
from math import log2

def Touch(index, width, lines, cache):
	hit = None
	for entry in cache:
		if entry == index - index % width:
			hit = entry
			break
	if hit != None:
		cache.remove(hit)
		cache.append(hit)
		return 0
	else:
		cache.append(index - index % width)
		if len(cache) > lines:
			cache.pop(0)
		return 1


width = 4
lines = 16

for N in [256, 512, 1024, 2048, 4096, 4096*2,4096*4]:
	count = 0
	cache = []
	comps = list(BitonicSort(range(N)))
	for i, j in comps:
		count += Touch(i, width, lines, cache)
		count += Touch(j, width, lines, cache)
	print(count/(N/width*(log2(N)-log2(width*lines))**2))





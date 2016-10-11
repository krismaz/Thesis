import random
import math
from Networks import *
from Layerize import layerize

N = 2**10

comparators = list(OddEvenMergesort(list(range(N))))

print(len(comparators))

inputs = list(range(N))
result = True
for i in range(1):
	random.shuffle(inputs)

	#print(inputs)

	#for c in comparators:
	#	if len(c) != 2:
	#		print(c)

	#for (i,j) in comparators: #Allow reverse comparators, important from bitonic sort
	#	if inputs[i] > inputs[j]:
	#		inputs[i], inputs[j] = inputs[j], inputs[i]

	for (i,j) in comparators: #Disallow reverse comparators, important for SimpleAKS
		inputs[min(i,j)], inputs[max(i,j)] = min(inputs[j], inputs[i]), max(inputs[j], inputs[i])

 	
	result = result and inputs == sorted(inputs)

print(result)


for c, (i, j) in enumerate(layerize(BitonicSort(range(8)),8)):
	pass
	#print('{0}: CE({1},{2})'.format(c, i, j))

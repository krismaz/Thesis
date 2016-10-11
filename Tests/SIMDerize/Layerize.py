#!/usr/bin/python3
def layerize(ops, n):
	layer = [-1]*n
	result = []
	for i,j in ops:
		free = max(layer[i], layer[j])+1
		result.append((free, (i,j)))
		layer[i], layer[j] = free, free
	return [o for _, o in sorted(result)]

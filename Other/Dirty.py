from PIL.Image import * 
from PIL.ImageDraw import * 
from Networks import *
from random import shuffle

size, blocks = 1024, 64

N = size*blocks

network = RandShellSort(range(N), True)

inputs = list(range(N))

shuffle(inputs)

lines = []

for (i,j) in network: #Allow reverse comparators, important from bitonic sort
		if j < 0:
			lines.append([sum(1 for k in inputs[i:i+size] if k>=N/2) for i in range(0, N//2, size)]+[sum(1 for k in inputs[i:i+size] if k<N/2) for i in range(N//2, N, size)])
		elif inputs[i] > inputs[j]:
			inputs[i], inputs[j] = inputs[j], inputs[i]

print(lines)

wh = 20

img = new('RGB', (blocks*wh+1, len(lines)*wh+1), (255, 255, 255))

drawing = Draw(img)

mm = max(max(line) for line in lines)

for i, line in enumerate(lines):
	for j, c in enumerate(line):
		low = (j*wh, i*wh)
		high = ((j+1)*wh, (i+1)*wh)
		colour = (255, 255-c*255 // mm, 255 - c*255 // mm)
		drawing.rectangle((low, high),colour, (0,0,0))

img.show()

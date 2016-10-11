from PIL.Image import * 
from PIL.ImageDraw import * 
from Networks import *
from Layerize import layerize

N = 8

w, h = 30, 30

network = BitonicSort(range(N), breaks = True)#[(0,2), (1,3), (0,1), (2,3), (1,2)]

layers = [-1]*N

pre = []

for i,j in network:
	print(i,j)
	if i < 0:
		layers[j[0]:j[1]] = [max(layers[j[0]:j[1]])]*(j[1]-j[0]+1)
		continue
	l = max(layers[i], layers[j]) + 1
	layers[min(i,j):max(i,j)+1] = [l]*(abs(j-i)+1)
	pre.append((l,i,j))

ww, hh = (max(layers)+2)*w, (N+1)*h

img = new('RGB', (ww, hh) , (255, 255, 255))

drawing = Draw(img)

for i in range(1,N+1):
	drawing.line([(0,i*h),(ww, i*h)], (128, 128, 128), 3)

r = 3

for l, i, j in pre:
	color = 0 if i<j else (255, 0, 0)
	drawing.line([((l+1)*w, (i+1)*h), ((l+1)*w, (j+1)*h)], color, 3)
	drawing.ellipse(((l+1)*w-r,(i+1)*h-r,(l+1)*w+r,(i+1)*h+r), fill=color)
	drawing.ellipse(((l+1)*w-r,(j+1)*h-r,(l+1)*w+r,(j+1)*h+r), fill=color)

img.save('network.png')
from math import log, e
from random import shuffle, randint

N = 2**9

print("Running with N = " + str(N))

q = 1

r = int(2 * log(N,2)/log(log(N,2), 2))

c = 2

p1Limit = int(log(N,2)**6)

p2Limit = int(64*(e**2)*log(N,2))+1

jump = 2*N

T = []
R = []

while(jump > p1Limit):
	T.append(jump)
	T.append(jump)
	R.append(c)
	R.append(c)
	jump = int(jump/2)

jump = p1Limit

while(jump > p2Limit):
	T.append(jump)
	R.append(r)
	jump = int(jump/2)

for i in range(1, p2Limit):
	T.append(1)
	R.append(1)


items = [i for i in range(N)]
shuffle(items)

# Sort an array a[0...n-1].
gaps = [511, 255, 127, 63, 31, 15, 7, 3, 1]

# Start with the largest gap and work down to a gap of 1 
# for k, gap in enumerate(gaps):
# 	for i in range(gap, N):
# 		j = i
# 		c = 0
# 		while j >= gap and c <= k*5:
# 			if items[j] < items[j-gap]:
# 				temp = items[j]
# 				items[j] = items[j-gap]
# 				items[j-gap]  = temp
# 			j = j - gap
# 			c += 1


#for gap in gaps:
#	for i in range(0, gap):
#		for j in range(i, N, gap):
#			for k in range(j, N, gap):
#				if items[j] > items[k]:
#					temp = items[j]
#					items[j] = items[k]
#					items[k]  = temp

for t, r in zip(T, R):
	for i in range(0, N-2):
		for _ in range(r):
			s = randint(i+1, min(N-1, i+t))
			if items[i] > items[s]:
				temp = items[i]
				items[i] = items[s]
				items[s]  = temp
	for i in range(N-1, 1, -1):
		for _ in range(r):
			s = randint(max(0, i-t), i-1)
			if items[s] > items[i]:
				temp = items[i]
				items[i] = items[s]
				items[s]  = temp

print(items)
print(sorted(items) == items)
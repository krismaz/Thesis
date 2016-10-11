from random import shuffle, randint

n=16

data = list(range(1,n+1))



shuffle(data)

print('digraph a{')
print('splines = "false";')
print('compound = "true"')
print('ranksep = "2.0";')

print('subgraph clusterA {')



print('label="Before";');
print('graph[style="dotted"];')

print('array1 [shape = "record", label = "'+ ' | '.join('<f' + str(i) + '> ' + str(j)  for i,j in enumerate(data)) + '"];')


print('array2 [shape = "record", label = "'+ ' | '.join('<f' + str(i + n) + '> ' + str(j)  for i,j in enumerate(data)) + '"];')


mates = list(range(n//2,n))

shuffle(mates)

comps = list(zip(range(0, n//2), mates))

for i,j in comps:
	print('array1:f' + str(i) + ':s -> ' + 'array2:f' + str(j + n) + ':n [dir="both"];') 
	if data[i] > data [j]:
		data[i], data[j] = data[j], data[i]


print('}')

print('subgraph clusterB {')

print('label="After";');
print('graph[style="dotted"];')

print('array3 [shape = "record", label = "'+ ' | '.join('<f' + str(i) + '> ' + str(j)  for i,j in enumerate(data)) + '"];')

print('}')

print('array2 -> array3 [ltail=clusterA, lhead=clusterB, style="bold", arrowhead="empty"];')
print('}')


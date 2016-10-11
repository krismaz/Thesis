from itertools import permutations

perms = list(permutations([1,2,3,4]))


revs = [tuple(t.index(i)+1 for i in range(1,5)) for t in perms]

s1 = 'static int perms[24] {' + ', '.join('_MM_SHUFFLE(' + ', '.join(str(e) for e in p) + ')' for p in perms)  +  '};'
s2 = 'static int revs[24] {' + ', '.join('_MM_SHUFFLE(' + ', '.join(str(e) for e in r) + ')' for r in revs) +  '};'

print(s1)
print(s2)
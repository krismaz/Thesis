import random
import math
import time

InvEps = 15

def EpsilonHalver(input1, input2):
	copy1, copy2 = list(input1), list(input2)
	for i in range(2*InvEps*int(math.log2(InvEps))+1):
		random.shuffle(copy1)
		random.shuffle(copy2)
		yield from zip(copy1, copy2)

def SpecificEpsilonHalver(input1, input2, inverse):
	copy1, copy2 = list(input1), list(input2)
	for i in range(2*inverse*int(math.log2(inverse))+1):
		random.shuffle(copy1)
		random.shuffle(copy2)
		yield from zip(copy1, copy2)

def EpsilonNearSort(inputs, eps, n):
	if len(inputs) <= max(eps*n, 1):
		pass
	else:
		eps1 = eps/math.log2(1/eps)
		inveps1 = int(1/eps1)+1
		yield from SpecificEpsilonHalver(inputs[0:len(inputs)//2], inputs[len(inputs)//2:], inveps1)
		yield from(EpsilonNearSort(inputs[0:len(inputs)//2], eps, n))
		yield from(EpsilonNearSort(inputs[len(inputs)//2:], eps, n))

def SillySort(inputs):
	k = 4
	if len(inputs) <= 64:
		yield from PrattSort(inputs)
	else:
		eps = 1/k
		yield from EpsilonNearSort(inputs, eps, len(inputs))
		i = 0
		step = len(inputs)//k
		for j in range(k-1):
			yield from SillySort(inputs[j*step:(j+2)*step])
		for j in reversed(range(k-1)):
			yield from SillySort(inputs[j*step:(j+2)*step])
		

def EpsilonSeparator(inputs, p):
	if p>0:
		yield from EpsilonHalver(inputs[0:len(inputs)//2],inputs[len(inputs)//2:])
		yield from EpsilonSeparator(inputs[0:len(inputs)//2], p-1)
		yield from EpsilonSeparator(inputs[len(inputs)//2:], p-1)

def Attenuate(A, B):
	if len(A) <= 4:
		yield from BitonicSort(A+B)
		return
	A1, A2, B1, B2 = A[:len(A)//2], A[len(A)//2:], B[:len(B)//2], B[len(B)//2:]
	yield from EpsilonHalver(A1, A2)
	yield from EpsilonHalver(B1, B2)
	yield from EpsilonHalver(A2, B1)
	yield from Attenuate(A2, B1)
	A21, A22, B11, B12 = A2[:len(A2)//2], A2[len(A2)//2:], B1[:len(B1)//2], B1[len(B1)//2:]
	yield from EpsilonHalver(A21, A22)
	yield from EpsilonHalver(B11, B12)
	yield from EpsilonHalver(A22, B12)
	yield from Attenuate(A22, B11)

def Reduce(A, B):
	if len(A) <= 8:
		yield from OddEvenMergesort(A+B)
		return
	yield from EpsilonHalver(A, B)
	yield from Attenuate(A,B)

def ZigZagSort(A):
	bucket = [list(A)]
	while len(bucket[0])>1:
		nbucket = []
		for b in bucket:
			left, right = b[:len(b)//2], b[len(b)//2:]
			yield from Reduce(left,right)
			nbucket += [left, right]
		for i in range(len(nbucket)-1):
			nbucket[i], nbucket[i+1] = nbucket[i+1], nbucket[i]
			yield from Reduce(nbucket[i], nbucket[i+1])
		for i in range(len(nbucket)-1, 1, -1):
			nbucket[i-1], nbucket[i] = nbucket[i], nbucket[i-1]
			yield from Reduce(nbucket[i-1], nbucket[i])
		bucket = nbucket

def BitonicMerge(inputs, breaks = False):
	if len(inputs) == 2:
		yield (inputs[0], inputs[1])
	else:
		yield from zip(inputs[:len(inputs)//2], inputs[len(inputs)//2:])
		if breaks:
			yield (-1,(min(inputs), max(inputs)))
		yield from BitonicMerge(inputs[:len(inputs)//2])
		yield from BitonicMerge(inputs[len(inputs)//2:])


def BitonicSort(inputs, breaks = False):
	if len(inputs) > 1:
		yield from BitonicSort(inputs[:len(inputs)//2])
		yield from BitonicSort(list(reversed(inputs[len(inputs)//2:])))
		if breaks:
			yield (-1,(min(inputs), max(inputs)))
		yield from BitonicMerge(inputs, breaks)

def OddEvenMerge(inputs):
	if len(inputs) == 2:
		yield (inputs[0], inputs[1])
	else:
		yield from OddEvenMerge(inputs[0::2])
		yield from OddEvenMerge(inputs[1::2])
		yield from zip(inputs[1:-1:2], inputs[2:-1:2])

def OddEvenMergesort(inputs):
	if len(inputs) > 1:
		yield from OddEvenMergesort(inputs[:len(inputs)//2])
		yield from OddEvenMergesort(inputs[len(inputs)//2:])
		yield from OddEvenMerge(inputs)

def PrattSort(inputs):
	i = len(inputs)//2
	while(i>0):
		j, run = i, True
		while(run):
			for k in range(len(inputs)-j):
				yield (inputs[k], inputs[k+j])
			j=(3*j)//2
			run = j % 3 == 0 and j < len(inputs)
		i=i//2

def ShakerSort(inputs):
	seq = [1,1]
	c = 1.7
	while(c+1 < len(inputs)):
		seq.append(int(c)+1)
		c*=1.7
	print(seq)
	for s in reversed(seq):
		for k in range(0, len(inputs)-s):
			yield (inputs[k], inputs[k+s])
		for k in range(len(inputs)-1, s-1, -1):
			yield (inputs[k-s], inputs[k])

def CompareRegions(input1, input2):
	shuffle = list(input2)
	random.shuffle(shuffle)
	yield from zip(input1, shuffle)


def RandShellSort(inputs, trace = False):
	i = len(inputs)//2
	while i > 0:
		if trace:
			yield (-1, -1)
		for j in range(len(inputs)//i - 1):
			yield from CompareRegions(inputs[i*j:i*(j+1)],inputs[i*(j+1):i*(j+2)])
		for j in reversed(range(len(inputs)//i - 1)):
			yield from CompareRegions(inputs[i*j:i*(j+1)], inputs[i*(j+1):i*(j+2)])
		for j in range(len(inputs)//i - 3):
			yield from CompareRegions(inputs[i*j:i*(j+1)],inputs[i*(j+3):i*(j+4)])
		for j in range(len(inputs)//i - 2):
			yield from CompareRegions(inputs[i*j:i*(j+1)],inputs[i*(j+2):i*(j+3)])
		for j in range(0, len(inputs)//i - 1, 2):
			yield from CompareRegions(inputs[i*j:i*(j+1)],inputs[i*(j+1):i*(j+2)])
		for j in range(1, len(inputs)//i - 1, 2):
			yield from CompareRegions(inputs[i*j:i*(j+1)],inputs[i*(j+1):i*(j+2)])
		i = i//2




class AKSNode:
	def __init__(self, d):
		self.depth = d
		self.buffer = []
		self.elements = []
		self.left = None
		self.right = None
		self.parent = self

	def receive(self, elements):
	 	self.buffer = self.buffer + elements

	def tick(self, t):
		self.elements = self.buffer
		self.buffer = []
		self.capacity = N*0.65**t*10**self.depth
		if self.left != None:
			self.left.tick(t)
		if self.right != None:
			self.right.tick(t)

	def tock(self):	
		yield from EpsilonSeparator(self.elements, 5)
		if self.left != None:
			yield from self.left.tock()
		else:
			self.left = AKSNode(self.depth+1)
			self.left.parent = self
		if self.right != None:
			yield from self.right.tock()
		else:
			self.right = AKSNode(self.depth+1)
			self.right.parent = self
		ends = int(self.capacity // 99)
		print(ends)
		self.parent.receive(self.elements[0:ends])
		self.parent.receive(self.elements[len(self.elements)-ends:])
		self.left.receive(self.elements[ends:len(self.elements)//2])
		self.right.receive(self.elements[len(self.elements)//2:len(self.elements)-ends])
		self.elements = []


def AKSSortSimple(inputs): #Note: Not correct or anything, but used for undertanding stuff
	root = AKSNode(0)
	root.receive(inputs)
	for i in range(7): #Should be O(log n) 
		root.tick(i)
		yield from root.tock()
	for i in range(0, N, 128):
		yield from PrattSort(inputs[i:i+128])

def PairwiseSort(inputs):
	if len(inputs) > 1:
		yield from zip(inputs[0::2], inputs[1::2])
		yield from PairwiseSort(inputs[0::2])
		yield from PairwiseSort(inputs[1::2])
		for m in range(int(math.ceil(math.log2(len(inputs))))-1, 0, -1):
			yield from zip(inputs[1::2], inputs[2**m::2])

def PeriodicMerge(inputs, m):
	if len(inputs) > 1 and m > 0:
		yield from zip(inputs[:len(inputs)//2], inputs[-1:len(inputs)//2-1:-1])
		yield from PeriodicMerge(inputs[:len(inputs)//2], m-1)
		yield from PeriodicMerge(inputs[len(inputs)//2:], m-1)

def PeriodicSort(inputs):
	for m in range(int(math.ceil(math.log2(len(inputs))))):
		yield from PeriodicMerge(inputs, m+2)

def ColumnSort(inputs, c):
	if len(inputs) < 64:
		yield from PrattSort(inputs)
	else:
		step = len(inputs) // c
		for i in range(0, len(inputs), step):
			yield from ColumnSort(inputs[i:i+step],c)
		transposed = [inputs[i::c] for i in range(c)]
		for i in range(c):
			yield from ColumnSort(transposed[i],c)
		for i in range(0, len(inputs), step):
			yield from ColumnSort(inputs[i:i+step],c)
		yield from ColumnSort(inputs[:step//2],c)
		for i in range(step//2, len(inputs), step):
			yield from ColumnSort(inputs[i:i+step], c)

def FourWayMerge(inputs):
	if len(inputs)>4:
		parts = [inputs[i::4] for i in range(4)]
		for part in parts:
			yield from FourWayMerge(part)
		yield from zip(parts[2], parts[0][2:])
		yield from zip(parts[3], parts[1][2:])
		yield from zip(parts[1], parts[0][1:])
		yield from zip(parts[3], parts[2][1:])
		yield from zip(parts[2], parts[0][1:])
		yield from zip(parts[3], parts[1][1:])
		yield from zip(parts[1][1:], parts[2][1:])#offset
		yield from zip(parts[3], parts[0][1:])
	elif len(inputs) == 2:
		yield (inputs[0], inputs[1])
	elif len(inputs) == 3:
		yield from [(inputs[0], inputs[1]), (inputs[0], inputs[2]), (inputs[1], inputs[2])]
	elif len(inputs) == 4:
		yield from [(inputs[0], inputs[2]), (inputs[1], inputs[3]), (inputs[0], inputs[1]), (inputs[2], inputs[3]), (inputs[1], inputs[2])]



def FourWaySort(inputs): 
	if len(inputs) <= 4:
		yield from OddEvenMergesort(inputs)
	else: 
		yield from FourWaySort(inputs[0:len(inputs)//4])
		yield from FourWaySort(inputs[len(inputs)//4:len(inputs)//2])
		yield from FourWaySort(inputs[len(inputs)//2:3*len(inputs)//4])
		yield from FourWaySort(inputs[3*len(inputs)//4:])
		yield from FourWayMerge(inputs)

def ThreeWayMerge(inputs):
	if len(inputs)>3:
		parts = [inputs[i::3] for i in range(3)]
		for part in parts:
			yield from ThreeWayMerge(part)
		yield from zip(parts[2], parts[0][2:])
		yield from zip(parts[1], parts[0][1:])
		yield from zip(parts[2], parts[1][1:])
		yield from zip(parts[2], parts[0][1:])
	elif len(inputs) == 2:
		yield (inputs[0], inputs[1])
	elif len(inputs) == 3:
		yield from [(inputs[0], inputs[1]), (inputs[0], inputs[2]), (inputs[1], inputs[2])]



def ThreeWaySort(inputs):
	if len(inputs) == 2:
		yield (inputs[0], inputs[1] )
	elif len(inputs) > 2: 
		yield from ThreeWaySort(inputs[0:len(inputs)//3])
		yield from ThreeWaySort(inputs[len(inputs)//3:2*len(inputs)//3])
		yield from ThreeWaySort(inputs[2*len(inputs)//3:])
		yield from ThreeWayMerge(inputs)

def TwoWayMerge(inputs):
	if len(inputs)>2:
		parts = [inputs[i::2] for i in range(2)]
		for part in parts:
			yield from TwoWayMerge(part)
		yield from zip(parts[1], parts[0][1:])
	else:
		yield (inputs[0], inputs[1])



def TwoWaySort(inputs):
	if len(inputs) > 1:
		yield from TwoWaySort(inputs[0:len(inputs)//2])
		yield from TwoWaySort(inputs[len(inputs)//2:])
		yield from TwoWayMerge(inputs)


def OddEvenChineseSort(inputs):
	N = len(inputs);
	p = N//2
	while p > 0:
		d, r = p, 0
		q = N//2
		while q >= p:
			for i in range(N-d):
				if (i & p) == r:
					yield (i, i+d)
			d, r, q = q - p, p, q//2
		p = p//2
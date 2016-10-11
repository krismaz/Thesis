#!/usr/bin/python3
import argparse
import re
from Layerize import layerize

parser = argparse.ArgumentParser(description='Simderizer')
parser.add_argument('-fname', default='BitonicSort', type=str, help='Input file with lines of (a, b)')
parser.add_argument('-UU', default='False', type=str, help='Allow Unaligned Operations')
parser.add_argument('-shuffle', default='False', type=str, help='Allow Shuffled Operations')
parser.add_argument('-layer', default='False', type=str, help='Allow Layer-Wise Reordering')
args = parser.parse_args()

AA = True
UU = args.UU == 'True'
shuffle = args.shuffle == 'True'
layer = args.layer == 'True'
SIMDSize = 4

commands = []

Buffer = ([], [])

def Sequence(A):
	for i in range(len(A)-1):
		if A[i] + 1 != A[i+1]:
			return False
	return True

def Disjoint(A, B):
	return len(set(A).intersection(B)) == 0

def Aligned(A):
	return A[0] % 4 == 0

def Unique(A):
	return len(set(A)) == len(A)

def Handle(i, j):
	global Buffer, commands
	Buffer[0].append(i)
	Buffer[1].append(j)
	if len(Buffer[0]) == SIMDSize:
		A, B = Buffer[0], Buffer[1]
		if AA and Sequence(A) and Sequence(B) and Disjoint(A,B) and Aligned(A) and Aligned(B):
			Buffer = ([],[])
			commands.append('CE4A({}, {})'.format(A[0],B[0]))
		elif AA and Sequence(A[::-1]) and Sequence(B[::-1]) and Disjoint(A,B) and Aligned(A[::-1]) and Aligned(B[::-1]):
			Buffer = ([],[])
			commands.append('CE4A({}, {})'.format(A[3],B[3]))
		elif UU and Sequence(A) and Sequence(B) and Disjoint(A,B):
			Buffer = ([],[])
			commands.append('CE4U({}, {})'.format(A[0],B[0]))
		elif UU and Sequence(A[::-1]) and Sequence(B[::-1]) and Disjoint(A,B):
			Buffer = ([],[])
			commands.append('CE4U({}, {})'.format(A[3],B[3]))
		elif shuffle and Disjoint(A,B) and Unique(A) and Unique(B):
			Buffer = ([],[])
			commands.append('CE4SHUF({}, {}, {}, {}, {}, {}, {}, {})'.format(*(A+B)))
		else:
			commands.append('CE({}, {})'.format(A[0],B[0]))
			Buffer = (A[1:], B[1:])



def Flush():
	global Buffer, commands
	for i, j in zip(Buffer[0], Buffer[1]):
		commands.append('CE({}, {})'.format(i,j))

with open(args.fname) as f:
    content = f.readlines()

operations = []

for line in content:
	match = re.match(r"\((\d+),(\d+)\)", line)
	operations.append((int(match.group(1)), int(match.group(2))))

if layer:
	n = max(max(i,j) for i,j in operations) + 1
	operations = layerize(operations,n)

for i,j in operations:
	Handle(i,j)

Flush()

for c, command in enumerate(commands):
	print('{}: {}'.format(c,command))
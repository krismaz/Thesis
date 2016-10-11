import os
import re
import subprocess
import language_check
import time


tool = language_check.LanguageTool('en-UK')
includedFiles = r'.*\.tex$'
content = []

badPatterns = [(r'\s()\\(ref|cite)', 'Space not \'~\' before ref/cite', []),
				(r'\$.*[^r]\*().*\$', 'Star Operator', []),
				(r'(s()ection|a()lgorithm|f()igure)( |~)\\ref', 'Lower case section/algorithm/figure', []),
				(r'[^/](cache M()iss|[cC]ache-()[mM]iss|\b\sC()ache miss)', 'Cache miss spelling', []),
				(r'[mM]iss?-()[pP]rediction', 'Misprediction error', []),
				(r'{figure}()\[', 'Bad Figure Placement', []),
				(r'\$.*[^\\]()log\b.*\$', 'Non-roman log in mathmode', []),
				(r'\\()mod[^{]', 'Wrong usage of \\mod', []),
				(r'((?!(Figure|Section|Table|Appendix).*)\ref)', 'Unnamed Reference', []),
				(r'\b([Aa]ppropriate|[Qq]uite|[Hh]ope|[Vv]ery [^h]|[Nn]ice|[Ll]ikely)()', 'Non-sciency word', []),
				(r'SSE4\.2()', 'Wrong SSE version', []),
				(r'for ()loop', 'Missing for loop hyphen', []),
				(r'()\\todo', 'Unfixed todo', ['thesis.tex']),
				(r'()compare-[eE]xchange', 'Unbigged Compare-Exchange', ['SIMD.tex']),
				(r'()>=|<=', 'Un-mathed comparison', ['SIMD.tex']),
				(r'this ()paper', 'Paper mentioned', []),
				(r'\\Require()', 'Require obsolete', []),
				(r'ceil\{1\.7()', '1.7 misuse',[]),
				(r'Simd|simd()', 'SIMD',['thesis.tex']),
				(r'O\(()', 'Big-O', [])
				]
sublime = r'C:\Program Files\Sublime Text 3\subl'

def langCheck(lines, fname, rname):
	for match in tool.check(''.join(lines)):
		if match.ruleId not in ['MORFOLOGIK_RULE_EN_UK', 'WHITESPACE_RULE', 'CURRENCY', 'COMMA_PARENTHESIS_WHITESPACE',  'EN_UNPAIRED_BRACKETS']:
			print(fname)
			print(match)

def check(lines, patterns, fname, rname):
	counter = 0
	for pattern, mes, exclude in badPatterns:
		if fname in exclude:
			continue
		for c, line in enumerate(lines):
			match =  re.search(pattern, line)
			if match:
				subprocess.call([sublime, '{}:{}:{}'.format(rname, c+1, match.start(1) + 1)])
				print(fname, '::', c+1, ' # ', mes)
				print(line)
				counter += 1
	return counter

counter = 0

for root, dirs, files in os.walk('tex'):
	for filename in files:
		if re.match(includedFiles, filename):
			with open(os.path.join(root, filename)) as f:
				content = f.readlines()
				counter += check(content, badPatterns, filename, os.path.join(root, filename))
				langCheck(content, filename, os.path.join(root, filename))
print(counter, 'total errors')
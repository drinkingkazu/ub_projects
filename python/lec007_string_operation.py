#
# Simple example script to learn about a string operation
#

import sys

k='abcdefg.txt'
print
print "We learn about a string operation by using this string: \"%s\"" % k
print 'Hit enter to continue.'
sys.stdin.readline()

# Finding a match in the prefix
print "Does it start with a char 'a'?",
if k.startswith('a'):
    print '... yes!'
else:
    print '... no!'
print 'Hit enter to continue.'
sys.stdin.readline()

# Finding a match in the suffix
print "Does it end with a char 'z'?",
if k.endswith('z'):
    print '... yes!'
else:
    print '... no!'
print 'Hit enter to continue.'
sys.stdin.readline()

# Finding if it contains a substring
print "Does it contain 'kazu'?",
if k.find('kazu')>=0:
    print '... yes! at the index %d' % k.find('kazu')
else:
    print '... no!'
print 'Hit enter to continue.'
sys.stdin.readline()
print "How about 'c'?",
if k.find('c')>=0:
    print '... yes! at the index %d' % k.find('c')
else:
    print '... no!'
print 'Hit enter to continue.'
sys.stdin.readline()

# Finding a substring
print "What is the first 3 chars in '%s?'" % k
print 'k[0:3] is', k[0:3] 
print 'Hit enter to continue.'
sys.stdin.readline()
print "What is the file suffix (i.e. trailing of last '.')?"
print "suffix: '%s'" % k[k.rfind('.'):len(k)]
print 'Hit enter to continue.'
sys.stdin.readline()

# Replace a substring
print "How does it look if I replace '.txt' substring into '.root'?"
print "Before:",k
print "After:",k.replace('.txt','.root')
print 'Hit enter to continue.'
sys.stdin.readline()

# Split into a list of strings
print "Can I split it into a list of substrings with a splitting point '.'?"
print "Before:",k
print "Splitted:",k.split('.')

# Striping a trailing white space
j='   hello world   '
print "I have '%s' and want to strip white space (both sides)?" % j
print "Before : '%s'" % j
print "After  : '%s'" % j.strip().rstrip()
print 

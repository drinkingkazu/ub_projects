
#
# Playing w/ list
#
import sys

print
print 'Making a list of integers 0=>9'
mylist = list(xrange(10))
print 'mylist',mylist
sys.stdin.readline()

print
print 'Making a list of 2x+1 for x in [0,10]'
mylist = [2*x+1 for x in xrange(10)]
print 'mylist',mylist
sys.stdin.readline()

print
print 'Making a list of even numbers between 0=>9'
mylist = [x for x in xrange(10) if x%2==0]
print 'mylist',mylist
sys.stdin.readline()

print
print 'Making a list of char representing even numbers between 0=>9'
mylist = [str(x) for x in xrange(10) if x%2==0]
print 'mylist',mylist
sys.stdin.readline()

print
print 'Making a list of list with length x for x [0,3]'
mylist = [list(xrange(x)) for x in xrange(4)]
print 'mylist',mylist
sys.stdin.readline()

print
print 'Reading digits from dummy.txt!'
mylist = [ int(x) for x in open('dummy.txt','r').read().split() if x.isdigit()]
print 'mylist',mylist

print
print 'Reading non-digits from dummy.txt!'
mylist = [ x for x in open('dummy.txt','r').read().split() if not x.isdigit()]
print 'mylist',mylist
print

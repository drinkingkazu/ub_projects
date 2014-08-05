#
# Simple module to show how to catch exception
#

#
# Example 1: catch a typical exception, ValueError
#

import sys
print "Let me try executing: int('a') ... and catch ValueError exception!"
print "hit enter to continue."
sys.stdin.readline()

k = 'a'
try:
    int(k)
except ValueError as e:
    print 'Caught ValueError with a message...'
    print e
print

#
# Example 2: catch using a generic exception base class
#

print "Let me try executing: int('a') ... and catch generic Exception!"
print "hit enter to continue."
sys.stdin.readline()

k = 'a'
try:
    int(k)
except Exception as e:
    print 'Caught a generic Exception with a message...'
    print e
print


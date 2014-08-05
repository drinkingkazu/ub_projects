#
# Lecture on data types.
#
import sys

print 'This module shows various data types in python.'
print 'Hit enter to continue.'
sys.stdin.readline()

#
# Integers
# 
print 'Example of integer declaration and addition!'
k=0      # direct assignment
j=int(1) # using a contrustor: handy when casting a type
print ' %d + %d = %d' % (k, j, k+j)
print "Hit enter to continue."
sys.stdin.readline()

#
# Floating points ... note python only have "double" equivalent of C++.
#
print 'Example of floating point declaration and addition!'
k=0.2        # direct assignment
j=float(1.2) # using a constructor: handy when casting a type
print ' %g + %g = %g' % (k, j, k+j)
print "Hit enter to continue."
sys.stdin.readline()

#
# String 
#
print 'Example of string declaration and addition!'
k='k'      # direct assignment
j=str('j') # using a constructor: handy when casting a type
print ' %s + %s = %s' % (k, j, k+j)
print "Hit enter to continue."
sys.stdin.readline()

#
# List ... ordered array like std::vector but you are not limited to
#          use only one type of values. You can mix different value
#          objects in the same list.
#
print 'Example of list declaration and addition!'
k=[0,1,2.0]  # Using a constructor & assignment at once
j=[]  # Using a constructor, then add elements
j.append('a')
j.append('b')
print '%s + %s = %s' % (k, j, k+j) # note you can use string casting here
print "Hit enter to continue."
sys.stdin.readline()
print 
print 'One more ... you can get a length of list by using len() function!'
print 'Length of',k,'is',len(k)
print "Hit enter to continue."
sys.stdin.readline()

#
# List advanced ... you can make a list of list
#
k=[k,k]
print 'Making a list of list:', k
print 'Hit enter to continue.'
sys.stdin.readline()

#
# Tuple ... a list that cannot be changed once defined
#
k=(0,1,2)
print 'Tuple:', k
print 'Let me try to change tuple element 0...'
try:
    k[0]=3
except TypeError:
    print 'Caught an exception: tuple element cannot be re-assigned.'
print 'Hit enter to continue.'
sys.stdin.readline()

#
# Dictionary ... an equivalent of std::map in C++.
#                Like list, a key/value is not restricted to a specific
#                type which is the case for std::map in C++.
#
k={'a':0, 'b':1} # using a constructor & assignment at once
j={}             # using a constructor, then adding elements
j['c']=2 
j['d']=3
print 'Map example:', k, 'and', j
print 'Hit enter to continue.'
sys.stdin.readline()

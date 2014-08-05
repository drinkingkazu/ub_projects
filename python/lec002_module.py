#
# Simple python script to show the concept of a "module"
#

#
# Step 0 import a module
#
print
print 'Step 0...'
import toy_func
print 'Imported:',toy_func
print 'Listing what is under',toy_func
for x in toy_func.__dict__.keys():
    print 'Found:',x

#
# Step 1 import from toy_func.py (file)
#

print
print 'Step 1...'
from toy_func import toy
print 'Imported:',toy,'from',toy_func
toy()

#
# Step 2 import from toy_module (directory)
#

print
print 'Step 2...'
import toy_module
from toy_module import another_toy
print 'Imported:',another_toy,'from',toy_module
another_toy()
print

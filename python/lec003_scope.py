#
# Example python script to learn "scope"
#
import sys

#
# The "scope" is a range of programs' view. By default, you are in
# "__main__" scope. To inspect this scope, we can use __main__ module.
#
print "By default, you are in the scope called '__main__'."
print "Let's inspect what we have in this '__main__' scope!"
print "Importing '__main__' module..."
print 'Hit enter to continue.'
sys.stdin.readline()

print
import __main__
print "Imported!"
print "Each module usually comes with '__dict__' dictionary variable."
print "'__dict__' contains a list of variable name (key) and value (value)"
print "in the dictionary format (i.e. map)."
print
print "Let's see what is in the key list of __main__.__dict__!"
print 'Hit enter to continue.'
sys.stdin.readline()

print
print __main__.__dict__.keys()

print
print "You see 'sys' module? This is because we imported sys in the beginning."
print "When you do 'import x', upon success 'x' becomes available in the"
print "scope you imported from."
print 'Hit enter to continue.'
sys.stdin.readline()

print
print "Let's try now import 'time' module, then print out __main__.__dict__ key list."
print 'Hit enter to continue.'
sys.stdin.readline()
import time
print 'Imported. Let me print out __main__.__dict__.keys()...'
print __main__.__dict__.keys()
print "See now 'time' became available? This is how namespace works!"
print

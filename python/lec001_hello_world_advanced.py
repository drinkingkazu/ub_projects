#
# Advanced hello world: learning I/O method using sys module
#

# "print" function does 2 things:
# 1. pipe following string to stdout stream
# 2. flush the stdout stream with trailing new line

print "Hello world ... using 'print' function"

# Let's learn about string operation as well.
msg_1="Hello"
msg_2="world"
msg_3="... hey I learned how to sprintf equivalent method!"

print "%s %s %s" % (msg_1,msg_2,msg_3)

#
# Next method: use sys module
#
import sys

# sys.stdout.write function simply pipe the argument string to stdout stream.
# It does not trail a new line unlike "print"! So we do it by ourselves.
sys.stdout.write("Hello world ... using 'sys.stdout.write()'\n")

# You can also use sys.stderr.write() to pipe to stderr stream
sys.stderr.write("Hello world ... using 'sys.stderr.write()'\n") 

#
# Use sys module to read user's input
#
print
print "OK! Let's read in your input using sys module..."
print
sys.stdout.write("Type whatever:")
user_input=sys.stdin.readline()

print "Your input is shown in the next line!"
print user_input

#
# Simple example to define a function
#
import sys

# Function definition is made by "def" declaration
def factorial(value):
    # This function computes a factorial of "value" input.
    
    # First, check if "value" is of an integer type.
    try:
        value=int(value)
        if value<=0:
            sys.stderr.write("Input value %s is not positive integer!\n" % value)
            return -1            
    except ValueError:
        sys.stderr.write("Input value %s is not integer!\n" % value)
        return -1

    # Let's compute a factorial of this number.
    ctr=1
    result=1
    while ctr<=value:
        result=result*ctr
        ctr+=1
    return result

# Function definition to call a factorial
def main():
    print "This is a function to compute factorial."

    while 1:
        sys.stdout.write("Enter a positive integer:")
        value=sys.stdin.readline().rstrip('\n') # We remove a 'newline' from input.
        result=factorial(value)
        if result<0:
            print "Try again!"
        else:
            print "%s! = %d" % (value,result)
        print

# Invoke main function when a user call this script from a terminal.
if __name__ == '__main__':
    main()

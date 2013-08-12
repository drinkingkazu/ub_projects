#
# Simple python script to demonstrate how one can "import" your module
#
import sys,__main__

#############################################
#
# Step 1 ... import a module
#
# Recall there is a function called "factorial" in lec006_define_function.py.
# We import that function here...
module_name='lec006_define_function'
print
print "We attempt a function import from the file %s.py" % module_name
print "Hit enter to continue."
sys.stdin.readline()
print
print "Current list of modules available in __main__ is shown below."
default_main_keys=__main__.__dict__.keys()
print default_main_keys
print "Hit enter to continue."
sys.stdin.readline()
print
print "Invoking 'import %s'" % module_name
import lec006_define_function
print "Updated list of modules available in __main__ is shown below."
updated_main_keys=__main__.__dict__.keys()
print updated_main_keys
print
print "You see following modules are added!"
for key in updated_main_keys:
    if not key in default_main_keys:
        print key
print
print "Hit enter to continue."
sys.stdin.readline()
print

#############################################
#
# Import a function from the module lec006_define_function
#
print "Here's a list of items available under %s module" % module_name
for key in lec006_define_function.__dict__.keys():
    print key
print "Hit enter to continue."
sys.stdin.readline()

print "Note you can access a function by typing 'lec006_define_function.factorial'"
print lec006_define_function.factorial
print "Hit enter to continue."
sys.stdin.readline()

print "Let us import the function into __main__..."
from lec006_define_function import factorial
print "... done! now we can access by simply typing 'factorial'"
print factorial
print "Hit enter to continue."
sys.stdin.readline()

print "Let's just run this function to get a factorial of 5..."
print factorial(5)
print "Alright, lesson done!"
print

#############################################
#
# Final remark on "where" python looks for a module to be imported
#
print
print "Here's a final remark ... where python looks for a module to import?"
print "python looks up all files under directories defined in $PYTHONPATH"
print "shell environment variable AND the current directory. We could import"
print "%s because it is under the current directory." % module_name
print "If you want to import it from somewhere else, you must add this"
print "directory to your $PYTHONPATH!"
print




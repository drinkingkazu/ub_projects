#
# Simple example to show a usage of "os" module
#
import sys,os

test_dir='temp'

####################################################################
#
# Step 1: Check if a directory exists.
#
print "Use os.path.isdir() to check if a directory '%s' alredy exists or not ..." % test_dir,
if os.path.isdir(test_dir):
    print 'it does!'
else:
    print 'it does not!'
print 'Hit enter to continue.'
sys.stdin.readline()

####################################################################
#
# Step 2: create a directory using os.mkdir()
#
print "Use os.mkdir() to create a directory '%s' (if it does not exist)" % test_dir

if os.path.isdir(test_dir):
    print ' ... directory already exists! nothing to be done.'

else:
    os.mkdir(test_dir)

    # Check again if it is created or not
    if os.path.isdir(test_dir):
        # success!
        print " ... created a dir: '%s'" % test_dir

    else:
        # failed .. exit the program
        print " ... failed to create a dir '%s'" % test_dir
        print "exiting..."
        sys.exit(1)
print 'Hit enter to continue.'
sys.stdin.readline()

####################################################################
#
# Step 3: Just for playing, create 100 empty files... we use shell function "touch"
#         through os.system() shell call.
#
print "For this lesson, create 100 files under '%s'..." % test_dir
for x in xrange(100):
    file_name='%s/file_%03d.txt' % (test_dir,x)
    shell_command='touch %s' % file_name
    os.system(shell_command)
    # Note, above three lines can be done in 1 line like this:
    # os.system('touch %s/file_%03d.txt' % (test_dir,x))
print 'Hit enter to continue.'
sys.stdin.readline()

####################################################################
#
# Step 4: Get a list of files under test_dir
#
print "Get a list of files in the '%s' directory" % test_dir
file_list=os.listdir(test_dir)
print "Found %d files!" % len(file_list)
print 'Hit enter to continue.'
sys.stdin.readline()

####################################################################
#
# Step 5: Save a list of files under test_dir in a text file!
#
print "Let's save a list of files in '%s' directory in '%s/files.db'" % (test_dir,test_dir)

# Get output file stream, fout
file_name='%s/files.db' % test_dir
fout = open(file_name, 'w') # First argument = filename, 2nd argument = file I/O mode. 'w'=write.

# Loop over the list of files, and write to the output.
for f in file_list:
    fout.write('%s\n' % f) # add a new line
fout.close()
print 'Hit enter to continue.'
sys.stdin.readline()

###################################################################
#
# Step 6: Check if the list of files in files.db matches with the directory contents
# 
print "Let's check if a list of files in '%s' matches with our original list!" % file_name
print "Read-in '%s' ..." % file_name
# Read in files.db contents
fin = open(file_name,'r') # This time, give 'r' as the 2nd argument to "read" a file.
# You can read in many ways. Here, we use 'read' function to read all at once.
contents = fin.read()
# We know contents have file names separated by a new line.
# So let's use "split" method
stored_file_list = contents.split()
print "Found %d files in '%s'" % (len(stored_file_list),file_name)
# Let's check if files in the list matches with the original list.
print "Looping over a list of files from '%s' to see if all are in the original list ..." % file_name
for f in stored_file_list:
    if not f in file_list:
        print "File '%s' not in the original list!" % f
print "... Done!"
print
print 'Hit enter to continue.'
sys.stdin.readline()
# Let's check if files in the original list exist in the stored list.
print "Looping over the original list of files to see if all are in the list from '%s' ..." % file_name
for f in file_list:
    if not f in stored_file_list:
        print "File '%s' not in the list from '%s'!" % (f,file_name)
print "... Done!"
print
print 'Hit enter to continue.'
sys.stdin.readline()

###################################################################
#
# Step 7: Remove a directory using os.rmdir()
# 
print "We remove '%s' directory before exit..." % test_dir
try:
    os.rmdir(test_dir)
except OSError:
    print "... caught an exception! Probably the dir '%s' is not empty :)" % test_dir
    print "... you can remove files one-by-one. But you can also use a shell command!"
    print "... invoking os.system('rm -rf %s') ..." % test_dir
    os.system('rm -rf %s' % test_dir)
print 'Hit enter to continue.'
sys.stdin.readline()

# Check if a directory is removed    
if os.path.isdir(test_dir):
    print "... ?! somehow the directory '%s' still exists!" % test_dir
else:
    print "... successfully removed '%s'!" % test_dir

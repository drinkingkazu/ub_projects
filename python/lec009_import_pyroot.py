#
# An example module to import PyROOT
#
import os,sys
print 
print "This script attempts to import PyROOT."
print "PyROOT module is provided by ROOT team to make ROOT C++ classes"
print "available under python interpreter. The module is defined under"
print "$ROOTSYS/lib/ROOT.py."
print
print "Review lec008_import.py ... then you should know that you have"
print "to append $ROOTSYS/lib/ directory to your $PYTHONPATH shell env."
print "variable!"
print

#
# Step 0 ... check if ROOTSYS is set and ROOT.py exists
#
if not 'ROOTSYS' in os.environ.keys():
    print 'ERROR: You must have set your $ROOTSYS environment variable first!'
    print '       Execute the following commands, then try this script again.'
    print 
    print '> export ROOTSYS=$WHEREVER_YOUR_ROOT_IS'
    print '> export PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH'
    print
    sys.exit(1)

elif not os.path.isfile('%s/lib/ROOT.py' % os.environ['ROOTSYS']):
    print 'ERROR: You do not have %s/lib/ROOT.py!' % os.environ['ROOTSYS']
    print '       This is odd (ROOT.py should come with the default installation of ROOT'
    print '       Without this file, you cannot use PyROOT :('
    print '       If you cannot resolve this, contact kazu!'
    sys.exit(1)

elif not 'PYTHONPATH' in os.environ.keys():
    print 'ERROR: You must have set your $PYTHONPATH environment variable first!'
    print '       Execute the following command, then try this script again.'
    print
    print '> export PYTHONPATH=$ROOTSYS/lib'
    print
    sys.exit(1)

else:
    root_lib_dir='%s/lib' % os.environ['ROOTSYS']
    pythonpath=os.environ['PYTHONPATH']
    if pythonpath.find(root_lib_dir)<0:
        print 'ERROR: You must include $ROOTSYS/lib in your $PYTHONPATH!'
        print '       Execute the following command, then try this script again.'
        print
        print '> export PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH'
        print
        sys.exit(1)

#
# Check if ROOT can be imported or not.
#
try:
    import ROOT
    print 'Successfully imported ROOT!'
    print
except ImportError:
    print 'Caught an exception of ROOT import failure!'
    print 'Not sure why? Contact kazu!'
    print

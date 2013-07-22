#!/usr/bin/python

#
# Very simple script to see how you can use your compiled
# C++ code (class/functions) in python. This routine accesses
# "foo" class defined in "simple" package.
#

# Step 1: import ROOT
import ROOT
# Step 2: load foo library
ROOT.gSystem.Load("libfoo")
# Now whatever defined in libfoo library became available under ROOT
# namespace. You can use it as ROOT.foo or make it available in the
# global name scope by doing this.
from ROOT import foo

# Step 3: Play
k=foo()
k.speak()

#
# Example PyROOT script to run analysis module, ana_base.
# The usage is same for inherited analysis class instance.
#

# Load libraries
import os, ROOT
from ROOT import gSystem
gSystem.Load("libAnalysis")

# Now import ana_processor & your class. For this example, ana_base.
from ROOT import ana_processor, ana_base

# Create ana_processor instance
my_proc=ana_processor()

# Set input root file: this is decoder output root file.
# This time, we use a sample file prepared.
my_proc.add_input_file("%s/dat/sample.root" % os.environ['MAKE_TOP_DIR'])

# Set output root file: this is a separate root file in which your
# analysis module can store anything such as histograms, your own TTree, etc.
my_proc.set_ana_output_file("myout.root")

# Create analysis class instance. For this example, ana_base.
# To show how one can run multiple analysis modules at once,
# we make multiple ana_base instance.

my_ana_1=ana_base()
my_ana_2=ana_base()
my_ana_3=ana_base()

# Add analysis modules to the processor

my_proc.add_process(my_ana_1)
my_proc.add_process(my_ana_2)
my_proc.add_process(my_ana_3)

# Let's run it.

my_proc.run()

# done!

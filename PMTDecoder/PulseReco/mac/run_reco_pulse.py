#
# Example PyROOT script to run analysis module, ana_base.
# The usage is same for inherited analysis class instance.
#

# Load libraries
import os, ROOT, sys
from ROOT import gSystem
gSystem.Load("libAnalysis")

# Now import ana_processor & your class. For this example, ana_base.
from ROOT import *

# Create ana_processor instance
my_proc=ana_processor()

# We want to read & write the data file
my_proc.set_io_mode(storage_manager.BOTH)

# We want to add reconstructed pulse data members
my_proc.set_data_to_write(DATA_STRUCT.PULSE_COLLECTION)

# Set input root file: this is decoder output root file.
# This time, we use a sample file prepared.
my_proc.add_input_file(sys.argv[1])

# Set output root file: this is a separate root file in which your
# analysis module can store anything such as histograms, your own TTree, etc.
my_proc.set_ana_output_file("ana.root")

my_proc.set_output_file("out.root")

# Create pulse reconstruction instance, and attach it to processor

preco = pulse_reco()
#preco.set_reco_algo(algo_fixed_window())
preco.set_reco_algo(algo_threshold())
preco.set_ped_algo(preco.kHEAD)
preco.set_ped_nsample_head(2)

my_proc.add_process(preco)

#my_proc.set_verbosity(MSG.DEBUG)

# Let's run it.

my_proc.run()

# done!

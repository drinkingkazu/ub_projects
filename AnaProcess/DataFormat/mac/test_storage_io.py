from ROOT import gSystem
gSystem.Load("libDataFormat")
from ROOT import *

k=storage_manager(storage_manager.WRITE)
k.set_out_filename("out.root")
k.open()

for x in xrange(100):
    
    for y in xrange(100):
        k.get_data(DATA_STRUCT.PMT_WF_COLLECTION).push_back(pmt_waveform(y))
    k.next_event()

k.close()

k.reset()

k.set_io_mode(storage_manager.READ)
k.add_in_filename("out.root")
k.open()

nevents=0
nreadout=0
while k.next_event():
    nevents+=1
    nreadout+=k.get_data(DATA_STRUCT.PMT_WF_COLLECTION).size()

print nevents
print nreadout

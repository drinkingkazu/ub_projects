from ROOT import gSystem
gSystem.Load("libdecoder")
from ROOT import event_waveform, pmt_waveform, storage_manager

k=storage_manager(storage_manager.WRITE)
k.set_filename("out.root")
k.open()

for x in xrange(100):
    
    for y in xrange(100):
        k.append_pmt_data(pmt_waveform(y))
    k.next_event()

k.close()

k.reset()

k.set_io_mode(storage_manager.READ)
k.set_filename("out.root")
k.open()

nevents=0
nreadout=0
while k.next_event():
    nevents+=1
    nreadout+=k.get_data().size()

print nevents
print nreadout

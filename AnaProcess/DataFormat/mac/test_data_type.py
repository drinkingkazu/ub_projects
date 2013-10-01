from ROOT import gSystem
gSystem.Load("libDataFormat")
from ROOT import pmt_wf_collection, pmt_waveform
event=pmt_wf_collection()

for x in xrange(100):
    ch=pmt_waveform(x,0,0,100)
    for y in xrange(ch.size()):
        ch.push_back(y)
    event.push_back(ch)

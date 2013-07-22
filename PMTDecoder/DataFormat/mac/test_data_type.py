from ROOT import gSystem
gSystem.Load("libdecoder")
from ROOT import event_waveform, pmt_waveform
event=event_waveform()

for x in xrange(100):
    ch=pmt_waveform(x,0,0,100)
    for y in xrange(ch.size()):
        ch.push_back(y)
    event.push_back(ch)

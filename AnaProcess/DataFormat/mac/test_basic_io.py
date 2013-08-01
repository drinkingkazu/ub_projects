from ROOT import gSystem,TFile,TTree
gSystem.Load("libDataFormat")
from ROOT import event_waveform, pmt_waveform

fout=TFile.Open("out.root","RECREATE")
t=TTree("tree","tree")
event=event_waveform()
t.Branch("data","event_waveform",event)
for x in xrange(100):
    ch=pmt_waveform(x,0,0,100)
    for y in xrange(ch.size()):
        ch.push_back(y)
    event.push_back(ch)
t.Fill()
t.Write()
#event.Write()
fout.Close()

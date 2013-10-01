from ROOT import gSystem,TFile,TTree
gSystem.Load("libDataFormat")
from ROOT import pmt_wf_collection, pmt_waveform

fout=TFile.Open("out.root","RECREATE")
t=TTree("tree","tree")
event=pmt_wf_collection()
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

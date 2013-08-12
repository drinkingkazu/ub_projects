#
# A simple script to play with PyROOT
#
import sys
try:
    import ROOT
except ImportError:
    print 'Failed to import PyROOT!'
    print 'Check your setup using lec009_import_pyroot.py!'
    print
    sys.exit(1)

#
# Let's make a histogram & canvas
#
from ROOT import *  # This line import EVERYTHING from ROOT
h1 = TH1D("h1","Example Histo; X; Factorial", 10,0.5,10.5)
c1 = TCanvas("c1","",600,500)
print "Created a histogram '%s' ... drawing it on a canvas '%s'" % (h1.GetName(),c1.GetName())
c1.cd()
h1.Draw()
c1.Update()
print 'Hit enter to continue.'
sys.stdin.readline()

#
# Let's import factorial function and fill the historam
#
print "Next, we import 'factorial' function from lec006_define_function.py."
print "Then we fill the histogram with factorial of the X-axis value!"
from lec006_define_function import factorial

for x in xrange(h1.GetNbinsX()):
    # Note ROOT histogram bin number starts from 1 while xrange array starts from 0.
    h1.SetBinContent(x+1, factorial(x+1))
gStyle.SetOptStat(0)
h1.SetFillStyle(3002)
h1.SetFillColor(kGray)
h1.SetLineWidth(2)
h1.SetLineColor(kBlue)
h1.Draw()
c1.SetLogy(1)
c1.Update()
print 'Hit enter to continue.'
sys.stdin.readline()

#
# Let's read-in an example TTree, "tree", from an root file, "example.root"
#
print "Let's read-in example TTree from example.root!"
fin=TFile.Open("example.root","READ")
if not fin:
    print
    print "TFile::Open failed! Check if example.root exists in this directory."
    print
    sys.exit(1)
tree=fin.Get("tree")
if not tree:
    print
    print "example.root opened, but did not find a TTree with a name 'tree'!"
    print "Check the file content and try again."
    print
    sys.exit(1)

print
print "This TTree contains two variables: index and value."
print "As an example, we make 2 of TH1D histogram to fill with each variable through TTree Draw."
print 'Hit enter to continue.'
sys.stdin.readline()

#
# Let's make a canvas & two histograms to draw ... use TTree::Draw() to fill the histogram
#
c2=TCanvas("c2","",600,500)
c3=TCanvas("c3","",600,500)

hIndex=TH1D("hIndex","Histogram of tree::index variable; Index; Entries",
            100,-0.5,99.5)

hValue=TH1D("hValue","Histogram of tree:value variable; Value; Entries",
            100,0,1e4)

for h in [hIndex,hValue]:
    h.SetLineWidth(2)
    h.SetLineColor(kRed)
    h.SetFillStyle(3002)
    h.SetFillColor(kGray)

c2.cd()
tree.Draw("index>>%s" % hIndex.GetName())
c2.Update()

c3.cd()
tree.Draw("value>>%s" % hValue.GetName())
c3.Update()

print
print 'Hit enter to continue.'
sys.stdin.readline()

#
# Try event loop in this TTree and make TGraph of data points
#
print 'Next, we loop over TTree and fill a TGraph with index vs. value data points.'

gCorr=TGraph(tree.GetEntries())
gCorr.SetTitle("Correlation of Index vs. Value; Index; Value") 
for x in xrange(tree.GetEntries()):
    tree.GetEntry(x)
    gCorr.SetPoint(x, tree.index, tree.value)
c4=TCanvas("c4","",600,500)
gCorr.SetMarkerStyle(20)
gCorr.SetMarkerColor(kCyan)
gCorr.Draw("AP")
c4.Update()

print
print 'Hit enter to continue.'
sys.stdin.readline()

#
# Try fitting a function to the created TGraph
#
print 'Fit a function to the filled TGraph.'
func=TF1("func","[0]+[1]*pow(x,[2])",0,100)
func.SetParName(0,"Offset")
func.SetParName(1,"Normalization")
func.SetParName(2,"Power Factor")
func.SetParameter(0,0)
func.SetParameter(1,1)
func.SetParameter(2,2)
func.SetLineWidth(2)
func.SetLineColor(kMagenta-7)
gStyle.SetOptFit(1)
gCorr.Fit("func","R")
gCorr.Draw("AP")
c4.SetGridx(1)
c4.SetGridy(1)
c4.Update()

print
print 'Hit enter to exit.'
sys.stdin.readline()

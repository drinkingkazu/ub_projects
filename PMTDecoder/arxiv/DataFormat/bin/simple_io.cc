#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include "event_waveform.hh"

int main(){

  //
  // Simple routine to make sure data format class can be written in/out.
  //

  // Event ID for testing.
  int event_id=1234;

  //
  // Step 1: write out an empty event.
  //

  // Create an empty data
  event_waveform* data=new event_waveform();
  data->set_event_id(event_id);

  // Save in TTree & File.
  TFile *fout=TFile::Open("tmp.root","RECREATE");
  TTree *tree=new TTree("tmp_tree",Form("Tree For %s",data->GetName()));
  tree->Branch(data->GetName(),data->GetName(),&data);
  tree->Fill();
  tree->Write();
  fout->Close();

  std::cout 
    << "Saved Tree..." << std::endl
    << "Setting the in-memory event id to something else..." << std::endl;
  data->set_event_id(4321);

  //
  // Step 2: read out the event from the saved file.
  //
  
  TFile *fin=TFile::Open("tmp.root","READ");
  tree = (TTree*)(fin->Get("tmp_tree"));
  tree->SetBranchAddress(data->GetName(),&data);
  tree->GetEntry(0);

  std::cout
    << Form("Reading event id from tree: %d", data->event_id())
    << std::endl;
  
  int return_val=0;
  if(data->event_id()==event_id) {
    std::cout << "SUCCESS!" << std::endl;
    return_val=0;
  }else{
    std::cout << "FAILURE!" << std::endl;
    return_val=1;
  }

  std::cout<<"Removing a tmporary file..."<<std::endl;
  gROOT->ProcessLine(".! rm tmp.root;");

  return return_val;
}



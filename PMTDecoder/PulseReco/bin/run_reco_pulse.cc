//
// Example C++ routine to run analysis module, ana_base.
// The usage is same for inherited analysis class instance.
//

#include <TSystem.h>
#include <PulseReco-TypeDef.hh>
#include <Analysis-TypeDef.hh>
#include <TString.h>
int main(int argc, char** argv){
  // Create ana_processor instance
  ana_processor my_proc;

  my_proc.add_input_file(argv[1]);

  my_proc.set_data_to_write(DATA_STRUCT::PULSE_COLLECTION);

  my_proc.set_output_file("out.root");

  my_proc.set_io_mode(storage_manager::BOTH);

  my_proc.set_ana_output_file("ana.root");

  my_proc.add_process(new pmtbaseline());

  pulse_reco* preco = new pulse_reco;

  preco->set_reco_algo(new algo_threshold);

  preco->set_ped_algo(preco->kHEAD);
  
  preco->set_ped_nsample_head(2);

  my_proc.add_process(preco);
  

  // Let's run it.

  my_proc.run();

  // done!
}

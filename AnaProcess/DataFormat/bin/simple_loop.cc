#include <TSystem.h>
#include <storage_manager.hh>
#include <constants.hh>

int main(){

  //
  // A simple routine to read a data file and perform an event loop.
  // This is a test routine for storage_manager class which interacts
  // decoder output root file. 
  //

  storage_manager kazu;

  // If you wish, change the message level to DEBUG.
  // Commented out by default.
  kazu.set_verbosity(MSG::DEBUG);
  

  // Step 0: Set I/O mode: we are reading in, so "READ"
  kazu.set_io_mode(storage_manager::READ);

  // Step 1: Set input file 
  kazu.add_in_filename(Form("%s/dat/sample.root",gSystem->Getenv("ANA_PROC_DIR")));

  // Step 2: Open a file.
  kazu.open();

  // Step 3: Check if a file is opened.
  if(!kazu.is_open()) {
    std::cerr << "File open failed!" << std::endl;
    return 0;
  }
  
  // Step 4: Check if it's ready to perform I/O
  if(!kazu.is_ready_io()) {
    std::cerr << "I/O preparation failed!" << std::endl;
  }

  //
  // Done preparation. Ready to perform an event loop.
  // We can read events indefinitely until we reach EOF by calling
  // storage_manager::next_event() function. This function returns
  // true upon successful read-in. It returns false if the retrieved
  // event is the last event entry in the input file.
  //
  // FYI: Step 2 to 4 can be ignored if you simply call next_event()
  //      function actually. next_event() function calls open() function
  //      if file is not yet opened. Above steps 2 to 4 are really just
  //      explicit testing purpose.
  //

  // Let's count # of events, and maybe loop over ADC samples to get the
  // highest ADC count.
  //event_waveform* data=0;
  int ctr=0;
  int highest_adc=-1;
  int highest_adc_pmt=-1;
  while(kazu.next_event()){

    ctr++;
    event_waveform *data=(event_waveform*)(kazu.get_data(DATA_STRUCT::WF_COLLECTION));

    // Let's loop over pmt-wise information. You can access this using
    // a index number such as data[0] to access the 1st entry. But here
    // I use an iterator.
    for(event_waveform::const_iterator pmt_iter(data->begin());
	pmt_iter!=data->end();
	++pmt_iter) {

      // Now you have pmt_waveform object in "(*pmt_iter)" expression!
      // Let's loop over ADC count in THIS PMT's waveform.
      for(pmt_waveform::const_iterator adc_iter((*pmt_iter).begin());
	  adc_iter!=(*pmt_iter).end();
	  ++adc_iter){
	if((*adc_iter) > highest_adc){
	  std::cout 
	    << Form("Found a new highest adc count: %d (Ch=%-3d)",
		    (*adc_iter),(*pmt_iter).channel_number())
	    << std::endl;
	  highest_adc=(*adc_iter);
	  highest_adc_pmt=(*pmt_iter).channel_number();
	}
      }// end of adc sample looping in THIS PMT.
    }// end of pmt_waveform looping in THIS event.
  }// end of event loop.

  std::cout 
    << std::endl
    << ctr << " events retrieved!" << std::endl
    << "Ch. " << highest_adc_pmt << " had the highest adc count: " << highest_adc
    << std::endl << std::endl;

  kazu.close();
  return 1;
}



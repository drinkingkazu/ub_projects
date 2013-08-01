//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class data_base+;
#pragma link C++ class pmt_waveform+;
#pragma link C++ class std::vector<pmt_waveform>+;
#pragma link C++ class event_waveform+;
#pragma link C++ class trig_info+;
#pragma link C++ class storage_manager+;
#pragma link C++ class pulse_info+;
#pragma link C++ class std::vector<pulse_info>+;
#pragma link C++ class pulse_collection+;
#pragma link C++ class user_info+;
#pragma link C++ class std::vector<user_info>+;
#pragma link C++ class user_collection+;

#endif

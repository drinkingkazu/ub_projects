//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class ana_processor+;
#pragma link C++ class std::set<UInt_t>+;
#pragma link C++ class reco_wf+;
#pragma link C++ class beamgate_debugger+;
#pragma link C++ class pmtbaseline+;
#pragma link C++ class pulse_viewer+;
#pragma link C++ class pmtbaseline_ana+;
#pragma link C++ class pulse_analyzer+;
#pragma link C++ class led_pulse_selector+;
#pragma link C++ class led_pulse_analyzer+;
#pragma link C++ class pulse_selector_base+;
#pragma link C++ class pulse_selector_simple+;
#pragma link C++ class waveform_sampler+;
//ADD_NEW_CLASS ... do not change this line
#endif





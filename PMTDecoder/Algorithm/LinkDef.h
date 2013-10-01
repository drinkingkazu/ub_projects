//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class algo_base+;
#pragma link C++ class algo_slow_readout_decoder+;
#pragma link C++ class algo_xmit_decoder+;
#pragma link C++ class algo_tpc_xmit+;
#pragma link C++ class algo_trig_decoder+;
#pragma link C++ class algo_fem_decoder_base+;
#endif

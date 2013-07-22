//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class ana_base+;
#pragma link C++ class ana_processor+;
#pragma link C++ class std::set<uint32_t>+;
#pragma link C++ class reco_wf+;
#pragma link C++ class beamgate_debugger+;
//ADD_NEW_CLASS ... do not change this line
#endif

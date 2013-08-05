/**
 * \file pmtbaseline_ana.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class pmtbaseline_ana
 *
 * @author kazuhiro
 */

/** \addtogroup Analysis

    @{*/

#ifndef PMTBASELINE_ANA_HH
#define PMTBASELINE_ANA_HH

#include "ana_base.hh"
#include <map>
#include <TH1D.h>

/**
   \class pmtbaseline_ana
   User custom analysis class made by kazuhiro
 */
class pmtbaseline_ana : public ana_base{

public:

  /// Default constructor
  pmtbaseline_ana(){ _name="pmtbaseline_ana"; _fout=0;};

  /// Default destructor
  virtual ~pmtbaseline_ana(){};

  /** IMPLEMENT in pmtbaseline_ana.cc!
      Initialization method to be called before the analysis event loop.
  */ 
  virtual bool initialize();

  /** IMPLEMENT in pmtbaseline_ana.cc! 
      Analyze a data event-by-event  
  */
  virtual bool analyze(storage_manager* storage);

  /** IMPLEMENT in pmtbaseline_ana.cc! 
      Finalize method to be called after all events processed.
  */
  virtual bool finalize();

protected:

  std::map<PMT::ch_number_t,TH1D*> _hArray_PedMean, _hArray_PedRMS, _hArray_Charge, _hArray_Peak;

};

#endif

/** @} */ // end of doxygen group 

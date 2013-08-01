/**
 * \file ana_base.hh
 *
 * \ingroup Analysis
 * 
 * \brief Base class def for all analysis classes
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Analysis

    @{*/

#ifndef ANA_BASE_HH
#define ANA_BASE_HH

#include <TFile.h>
#include "event_waveform.hh"
#include "storage_manager.hh"

/**
   \class ana_base
   A base class for analysis modules to be operated with event_waveform
   data class instance. 
 */
class ana_base : public decoder_base{

public:

  /// Default constructor
  ana_base(){ _name="ana_base"; _fout=0;};

  /// Default destructor
  virtual ~ana_base(){};

  /// Initialization method to be called before analyze any data
  virtual bool initialize(){return true;};

  /// Analyze a data event-by-event  
  virtual bool analyze(storage_manager* data){return (bool)data;};

  /// Finalize method to be called after all events processed.
  virtual bool finalize(){return true;};

  /// A setter for analysis output file poitner
  void set_output_file(TFile* fout){_fout=fout;};

protected:

  TFile* _fout; ///< Analysis output file pointer
};

#endif

/** @} */ // end of doxygen group 

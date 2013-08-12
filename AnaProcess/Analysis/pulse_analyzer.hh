/**
 * \file pulse_analyzer.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class pulse_analyzer
 *
 * @author kazuhiro
 */

/** \addtogroup Analysis

    @{*/

#ifndef PULSE_ANALYZER_HH
#define PULSE_ANALYZER_HH

#include <TH1D.h>
#include <TCanvas.h>
#include <TLine.h>
#include <set>
#include <algorithm>
#include <map>
#include <vector>
#include "ana_base.hh"
#include <TH1D.h>
#include <TH2D.h>
#include <math.h>
#include <TGraph.h>
#include <TGraphErrors.h>


#include "ana_base.hh"

/**
   \class pulse_analyzer
   This is a class to visualize reconstructed pulse information together with the raw waveform on a histogram.
   This helps to debug/understand pulse reconstruction algorithm output.
   It is meant to be used interactively and not in a batch mode as it does not store any objects in an output.
   It allows a user to specify a range for various pulse-wise and event-wise parameters so that one can
   filter out irrelevant pulses from visualization routine (which can take time).
   See Analysis/mac/pulse_analyzer.py for an example usage.
 */
class pulse_analyzer : public ana_base{

public:

  /// Default constructor
  pulse_analyzer();

  /// Default destructor
  virtual ~pulse_analyzer(){};

  /// Initialization method to be called before the analysis event loop.
  virtual bool initialize();

  /// Analyze a data event-by-event  
  virtual bool analyze(storage_manager* storage);

  /// Finalize method to be called after all events processed.
  virtual bool finalize();

  /// Clear event
  void clear_event();

  /** CORE METHOD 1
      Given the channel number, this method iterates over the locally stored
      values for a reconstructed pulse. It visualize these information on
      a canvas with the original waveform which is returned as TH1D object pointer.
      The canvas can be retrieved through pulse_viewer::get_canvas() method after
      you call this function.
   */

  PMT::word_t _event_id;   ///< event id holder
  double      _sum_charge; ///< event-wise summed charge holder
  double      _sum_peak;   ///< event-wise summed peak height holder
  uint32_t    _npulse;     ///< event-wise number of reco-ed pulse holder
  std::set<PMT::ch_number_t>                           _channels;             ///< set of channel numbers for reco-ed pulses
  std::set<PMT::ch_number_t>::iterator                 _ch_iter;              ///< internal iterator for _channels member
  /** Map of pulse count: the first element in the pair holds the total reco-ed pulse count while the second
      element holds the internal index to be updated by pulse_viewer::next_pulse() and pulse_viewer::previous_pulse() methods.
  */
  TH1D* amplitude;
};

#endif

/** @} */ // end of doxygen group 

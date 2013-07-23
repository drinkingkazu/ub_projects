/**
 * \file pulse_viewer.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class pulse_viewer
 *
 * @author kazuhiro
 */

/** \addtogroup Analysis

    @{*/

#ifndef PULSE_VIEWER_HH
#define PULSE_VIEWER_HH

#include <TH1D.h>
#include <TCanvas.h>
#include <TLine.h>
#include <set>
#include <algorithm>

#include "ana_base.hh"

/**
   \class pulse_viewer
   User custom analysis class made by kazuhiro
 */
class pulse_viewer : public ana_base{

public:

  /// Default constructor
  pulse_viewer();

  /// Default destructor
  virtual ~pulse_viewer(){};

  /** IMPLEMENT in pulse_viewer.cc!
      Initialization method to be called before the analysis event loop.
  */ 
  virtual bool initialize();

  /** IMPLEMENT in pulse_viewer.cc! 
      Analyze a data event-by-event  
  */
  virtual bool analyze(const storage_manager* storage);

  /** IMPLEMENT in pulse_viewer.cc! 
      Finalize method to be called after all events processed.
  */
  virtual bool finalize();

  const TCanvas* get_canvas() const {return _cWF;};
  const std::set<PMT::ch_number_t> get_hit_channels() const {return _channels;};
  PMT::ch_number_t next_channel();
  short get_npulse(PMT::ch_number_t ch);
  TH1D* next_pulse(PMT::ch_number_t ch);
  TH1D* previous_pulse(PMT::ch_number_t ch);

protected:

  void clear_map();
  void clear_viewer();
  TH1D* get_waveform(PMT::ch_number_t ch, size_t index);
  void add_channel_entry(PMT::ch_number_t ch);

  std::set<PMT::ch_number_t>                           _channels;
  std::set<PMT::ch_number_t>::iterator                 _ch_iter;
  std::map<PMT::ch_number_t,std::pair<short,short> >   _pulse_count;
  std::map<PMT::ch_number_t,std::vector<PMT::word_t> > _pulse_frame_id;
  std::map<PMT::ch_number_t,std::vector<PMT::word_t> > _pulse_sample_number;
  std::map<PMT::ch_number_t,std::vector<double> >      _pulse_tstart;
  std::map<PMT::ch_number_t,std::vector<double> >      _pulse_tend;
  std::map<PMT::ch_number_t,std::vector<double> >      _pulse_amp;  
  std::map<PMT::ch_number_t,std::vector<double> >      _pulse_charge;
  std::map<PMT::ch_number_t,std::vector<double> >      _pulse_pedbase;
  std::map<PMT::ch_number_t,std::vector<double> >      _pulse_pedrms;
  
  //std::map<PMT::ch_number_t,std::vector< std::vector<PMT::ch_adc_t> > > _waveforms;
  std::map<PMT::ch_number_t,std::map<PMT::word_t,std::map<PMT::word_t,std::vector<PMT::ch_adc_t> > > > _waveforms;

  PMT::word_t _event_id;
  TCanvas* _cWF;
  TH1D* _hWF;
  TLine *_lStart, *_lEnd, *_lBase, *_lRMSTop, *_lRMSBottom, *_lTop;
  
};

#endif

/** @} */ // end of doxygen group 

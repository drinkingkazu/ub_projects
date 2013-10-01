/**
 * \file pmt_waveform.hh
 *
 * \ingroup DataFormat
 * 
 * \brief PMT-wise data class def header file.
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/
#ifndef PMT_WAVEFORM_HH
#define PMT_WAVEFORM_HH

#include "data_base.hh"

/**
 \class pmt_waveform 
 PMT-wise data member class to hold a collection of ADC samples.
*/
class pmt_waveform : public std::vector<PMT::ch_adc_t>, 
		     public data_base {

public:

  /// Default constructor
  pmt_waveform(PMT::ch_number_t   ch        = PMT::INVALID_CH,
	       PMT::word_t        ch_frame  = PMT::INVALID_WORD,
	       PMT::DISCRIMINATOR disc_id   = PMT::DISC_MAX,
	       PMT::word_t        timeslice = PMT::INVALID_WORD,
	       size_t len=0) 
    : std::vector<PMT::ch_adc_t>(len), 
      data_base(),
      _channel_number(ch), 
      _channel_frame_id(ch_frame),
      _timeslice(timeslice),
      _disc_id(disc_id)
  {};

  /// Default copy constructor
  pmt_waveform(const pmt_waveform& original)
    : std::vector<PMT::ch_adc_t>(original),
      data_base(original),
      _channel_number(original._channel_number),
      _channel_frame_id(original._channel_frame_id),
      _timeslice(original._timeslice),
      _disc_id(original._disc_id)
  {};

  /// Setter for the channel number
  void set_channel_number   (PMT::ch_number_t ch)   {_channel_number=ch;};

  /// Setter for the channel frame ID number
  void set_channel_frame_id (PMT::word_t id)        {_channel_frame_id=id;};

  /// Setter for the channel discriminator ID number
  void set_disc_id          (PMT::DISCRIMINATOR id) {_disc_id=id;};

  /// Setter for the timeslice number
  void set_timeslice        (PMT::word_t t)         {_timeslice=t;};

  /// Getter for the channel number
  PMT::ch_number_t channel_number() const {return _channel_number;};

  /// Getter for the channel frame ID number
  PMT::word_t channel_frame_id()    const {return _channel_frame_id;};

  /// Getter for the discriminator ID number
  PMT::DISCRIMINATOR disc_id()      const {return _disc_id;};

  /// Getter for the timeslice number
  PMT::word_t timeslice()           const {return _timeslice;};

  /// Method to clear currently held data contents in the buffer
  virtual void clear_data();

  /// Default destructor
  ~pmt_waveform(){};


private:

  /// Internal function to reset variables.
  void init_vars();

  PMT::ch_number_t _channel_number; ///< Channel number
  PMT::word_t _channel_frame_id;    ///< Channel frame ID
  PMT::word_t _timeslice;           ///< Timeslice number
  PMT::DISCRIMINATOR _disc_id;      ///< Discriminator ID number
  
  ////////////////////////
  ClassDef(pmt_waveform,3)
  ////////////////////////
};



/**
 \class pmt_wf_collection 
 Event-wise data member class to hold a collection of ch-wise data members
*/
class pmt_wf_collection : public std::vector<pmt_waveform>, 
			  public data_base {
  
public:

  /// Default constructor ... provide an option to set the length of ch-wise data
  pmt_wf_collection(size_t len = 0) :
    std::vector<pmt_waveform>(len), 
    data_base()
  {};

  /// Default copy constructor needed to avoid memory leak in ROOT streamer
  pmt_wf_collection(const pmt_wf_collection& original)
    : std::vector<pmt_waveform>(original),
      data_base(original),
      _event_id(original._event_id),
      _event_frame_id(original._event_frame_id),
      _module_address(original._module_address),
      _module_id(original._module_id),
      _channel_header_count(original._channel_header_count),
      _checksum(original._checksum),
      _nwords(original._nwords)
  {};

  /// Default destructor
  ~pmt_wf_collection(){};

  /// Setter for the envet id number
  void set_event_id(PMT::word_t id)       {_event_id=id;};

  /// Setter for the event frame number
  void set_event_frame_id(PMT::word_t id) {_event_frame_id=id;};

  /// Setter for the module address from which this event is read-out
  void set_module_address(PMT::word_t id) {_module_address=id;};

  /// Setter for the module ID
  void set_module_id(PMT::word_t id)      {_module_id=id;};

  /// Setter for the channel's header count
  void set_channel_header_count(size_t n) {_channel_header_count=n;};

  /// Setter for the checksum
  void set_checksum(PMT::word_t sum)      {_checksum=sum;};

  /// Setter for the # words read out
  void set_nwords(PMT::word_t n)          {_nwords=n;};

  /// Setter for the trigger frame id
  void set_trigger_frame_id(PMT::word_t id) {_trigger_frame_id=id;};

  /// Setter for the trigger timeslice
  void set_trigger_timeslice(PMT::word_t n) {_trigger_timeslice=n;};

  /// Getter for the event ID
  PMT::word_t event_id()       const {return _event_id;};

  /// Getter for the frame ID
  PMT::word_t event_frame_id() const {return _event_frame_id;};

  /// Getter for the module address
  PMT::word_t module_address() const {return _module_address;};

  /// Getter for the module ID
  PMT::word_t module_id()      const {return _module_id;};

  /// Getter for the channel header count
  size_t      channel_header_count() const {return _channel_header_count;};

  /// Getter for the checksum
  PMT::word_t checksum()       const {return _checksum;}; 

  /// Getter for the number of read words
  PMT::word_t nwords()         const {return _nwords;};

  /// Getter for the trigger frame id
  PMT::word_t trigger_frame_id() const {return _trigger_frame_id;};

  /// Getter for the trigger time slice
  PMT::word_t trigger_timeslice() const {return _trigger_timeslice;};

  /// A function to reset data member variables
  virtual void clear_data();


private:

  void init_vars();
  /// Actual implementation function of resetting variables

  PMT::word_t _event_id;             ///< event ID number
  PMT::word_t _event_frame_id;       ///< event frame ID number
  PMT::word_t _module_address;       ///< module address number
  PMT::word_t _module_id;            ///< module ID number
  size_t      _channel_header_count; ///< channel header count

  PMT::word_t _trigger_frame_id;     ///< trigger frame id
  PMT::word_t _trigger_timeslice;    ///< trigger time slice

  PMT::word_t _checksum;             ///< checksum of readout events
  PMT::word_t _nwords;               ///< # of event words readout

  ///////////////////////////
  ClassDef(pmt_wf_collection,3)
  //////////////////////////
};

#endif

/** @} */ // end of doxygen group 

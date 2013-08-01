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

#endif

/** @} */ // end of doxygen group 

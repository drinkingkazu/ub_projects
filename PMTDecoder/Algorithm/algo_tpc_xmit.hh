/**
 * \file algo_tpc_xmit.hh
 *
 * \ingroup Algorithm
 * 
 * \brief Class def header of the decoder algorithm for slow readout
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Algorithm

    @{*/
#ifndef ALGO_TPC_XMIT_HH
#define ALGO_TPC_XMIT_HH

#include "algo_base.hh"
#include "algo_fem_decoder_base.hh"

/**
   \class algo_tpc_xmit
   Implementation for TPC XMIT data stream based on algo_fem_decoder_base class.

   There're lots of common things among PMT/TPC XMIT decoder, and ideally this class
   inherits from that of PMT. This is purely historical reason that I decided to develop
   this as a separate class w/o making such inheritance. It turned out PMT has a lot more
   information (not data volume) than TPC. It does not make sense to let TPC decoder
   class inherit from PMT decoder class (as TPC decoder is simpler). 
   This calls for a design review. For now, I omit this design issue and go ahead to
   implement a working version of tpc decoder.

   It expects a stream of 32-bit words (ordered) to be fed into
   process_word method. This class takes care of decoding & grouping
   channel/event wise data & storing them.
 */
class algo_tpc_xmit : public algo_fem_decoder_base {

public:

  /// Default constructor
  algo_tpc_xmit();

  /// Default destructor
  virtual ~algo_tpc_xmit(){};


  /// Override of algo_base::get_word_class method
  virtual inline PMT::PMT_WORD get_word_class(const PMT::word_t word) const {
    // One of core functions to identify PMT binary word format
    if( (word & 0xffffffff) == 0xffffffff )
      return PMT::EVENT_FIRST_HEADER;
    else if( (word & 0xf0000000) == 0xe0000000 )
      return PMT::EVENT_LAST_WORD;
    if( (word & 0xffff) == 0xffff )
      return PMT::EVENT_HEADER;
    else if( (word & 0xf000) == 0xf000 )
      return PMT::EVENT_HEADER;
    else if( (word & 0xf000) == 0x4000 )
      return PMT::CHANNEL_HEADER;
    else if( (word & 0xf000) == 0x0000 ||
	     (word & 0xf000) == 0x8000   )
      return PMT::CHANNEL_WORD;
    else if( (word & 0xf000) == 0x5000 )
      return PMT::CHANNEL_LAST_WORD;
    else
      return PMT::UNDEFINED_WORD;
  };

protected:

  virtual bool check_event_quality();
  void clear_event();
  bool store_event();

  virtual bool process_word(PMT::word_t word);

  virtual bool process_ch_word          (const PMT::word_t word, PMT::word_t &last_word);

  bool decode_ch_word                   (const PMT::word_t word, PMT::word_t &last_word);

  virtual bool handle_unexpected_header (const PMT::word_t word, PMT::word_t &last_word);

  virtual bool handle_failure_ch_word   (const PMT::word_t word, PMT::word_t &last_word);

  virtual void reset();

  virtual inline bool is_compressed(const PMT::word_t word){ return ( word & 0xf000 ); };

  /**
     A method to process 32-bit end-of-fem word. Nothing to be done really.
  */
  virtual bool process_fem_last_word(const PMT::word_t word,
				     PMT::word_t &last_word);

  /**
     A method to process 32-bit end-of-event word. Store event @ this point.
  */
  virtual bool process_event_last_word(const PMT::word_t word,
				       PMT::word_t &last_word);

protected:

  tpc_waveform       _ch_data;

  tpc_wf_collection* _event_data;

};

#endif

/** @} */ // end of doxygen group 

/**
 * \file algo_trig_decoder.hh
 *
 * \ingroup Algorithm
 * 
 * \brief Class def header for algo_trig_decoder class
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Algorithm

    @{*/

#ifndef ALGO_TRIG_DECODER
#define ALGO_TRIG_DECODER

#include "algo_base.hh"

/**
   \class algo_trig_decoder
   Algorithm to decode trigger words
*/
class algo_trig_decoder : public algo_base {

public:

  /// Default constructor 
  algo_trig_decoder();

  /// Default destructor 
  virtual ~algo_trig_decoder(){}; 
  
  /// Core function: expects a stream (ordered) of 32-bit words from PMT readout data. 
  /** A parent process to run this decoder should pass a stream of words to this method. 
      In this framework, a parent class should be decoder_manager for decoding, for instance.
  */
  virtual bool process_word(PMT::word_t word);

  /** A method to check event quality upon saving in the output file.
   */
  virtual bool check_event_quality();

  /// A method to clear event-wise infomration ... called upon filling each event in output.
  void clear_event_info();

  /// Override a function to prepare for new processing
  virtual void reset();

protected:

  bool decode_trigger_words(PMT::word_t *trigger_words);

  static const size_t TRIGGER_WORD_COUNT=3;

  trig_info *_data;

  uint32_t _nevents;
  size_t   _trigger_word_count;
  PMT::word_t _trigger_words[TRIGGER_WORD_COUNT];

};
#endif

/** @} */ // end of doxygen group

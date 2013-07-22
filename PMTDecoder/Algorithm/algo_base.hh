/**
 * \file algo_base.hh
 *
 * \ingroup Algorithm
 * 
 * \brief Algorithm base class, algo_base definition header file
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Algorithm

    @{*/

#ifndef ALGO_BASE
#define ALGO_BASE

#include "storage_manager.hh"

/**
   \class algo_base
   Decoder algorithm base class. 
   May need a little make up to be a base for encdoer algorithms.
*/
class algo_base : public decoder_base {

public:

  /// Default constructor 
  algo_base() { _name="algo_base"; _storage=0; _debug_mode=false;};

  /// Default destructor 
  virtual ~algo_base(){}; 

  /// Core function: expects a stream (ordered) of 32-bit words from PMT readout data. 
  /** A parent process to run this decoder should pass a stream of words to this method. 
      In this framework, a parent class should be decoder_manager for decoding, for instance.
      Children should implement to perform (1) decoding and (2) storing event data upon
      this function call.
  */
  virtual bool process_word(PMT::word_t word)=0;
  //{return true;};

  /// Function called just after processing word.
  virtual void finalize(){};

  /// Virtual member: this should return word type defined in FORMAT::PMT_WORD.
  virtual inline FORMAT::PMT_WORD get_word_class(const PMT::word_t word) const
  {if(word) return FORMAT::UNDEFINED_WORD; else return FORMAT::UNDEFINED_WORD;};

  /// Run utility: set debug mode. Children class defines what-to-do in debug mode.
  void set_debug_mode(bool doit){_debug_mode=doit;};

  /// Expects a storage pointer to be provided from a parent class. 
  void set_storage_ptr(storage_manager* storage){_storage=storage;};

  /** A method to check event quality upon saving in the output file.
      Children should implement.
   */
  virtual bool check_event_quality() {return false;};

  /// Function to reset variables for new usage
  virtual void reset(){ init_checker_info(); _storage=0;};
  
protected:

  /// Function to initialize checker information
  virtual void init_checker_info();

protected:
  //
  // Run control 
  //
  bool    _debug_mode;    ///< Debug mode boolean holder

  //
  // Constants
  //
  static const size_t BEAM_REF_CHANNEL=39; ///< Defines a reference channel for beamgate.
  static const size_t BEAM_NWORDS=21;      ///< Defines # of minimum words to identify beamgate event.
  static const size_t FRAME_WIDTH=8192;    ///< Defines # of ADC samples per readout frame.

  //
  // Data
  // 
  storage_manager* _storage;  ///< Data storage pointer to be provided by a parent process.

  //
  // Checker variables
  //
  PMT::word_t _checksum; ///< data checksum
  PMT::word_t _nwords;   ///< number of data word counts processed
  
};
#endif

/** @} */ // end of doxygen group

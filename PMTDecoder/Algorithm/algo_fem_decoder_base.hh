/**
 * \file algo_fem_decoder_base.hh
 *
 * \ingroup Algorithm
 * 
 * \brief Class def header of the decoder algorithm for slow readout
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Algorithm

    @{*/
#ifndef ALGO_FEM_DECODER_BASE_HH
#define ALGO_FEM_DECODER_BASE_HH

#include "algo_base.hh"

/**
   \class algo_fem_decoder_base
   Abastract base class for decoding FEM module data stream.

   It expects a stream of 32-bit words (ordered) to be fed into
   process_word method. This class takes care of decoding & grouping
   channel/event wise data & storing them.

   This base class only implements the overall structure of decoding.
   It has two types of processing path: either event header or channel data word.
   The FEM header is common among TPC and PMT as they share the same FEM module.
   Channel data word is defined differently as data packet FPGA code is unique to each.
   Children class must implement channel-wise decoding methods.
 */

class algo_fem_decoder_base : public algo_base {

public:

  /// Processing status enum for decision making
  enum PROCESS_FLAG{

    READ_HEADER=0, ///< Process status flag ... reading event header 

    READ_CHANNEL   /// Process status flag ... reading channel adc info 

  };

  /// A local data holder struct for FEM header information
  struct InfoFEM_t {

    PMT::word_t module_address;      ///< Module address
    PMT::word_t module_id;           ///< Module id number
    PMT::word_t event_id;            ///< Event counter 
    PMT::word_t event_frame_id;      ///< Data packet frame number
    PMT::word_t trigger_frame_id;    ///< Trigger frame number
    PMT::word_t trigger_timeslice;   ///< Trigger sample number
    PMT::word_t nwords;              ///< Number of words in an event
    PMT::word_t checksum;            ///< Data checksum

    void reset(){
      
      module_address    = PMT::INVALID_WORD;
      module_id         = PMT::INVALID_WORD;
      event_id          = PMT::INVALID_WORD;
      event_frame_id    = PMT::INVALID_WORD;
      trigger_frame_id  = PMT::INVALID_WORD;
      trigger_timeslice = PMT::INVALID_WORD;
      nwords            = 0;
      checksum          = 0;
    }

    InfoFEM_t(){ reset(); }

  };

public:
  
  /// Default constructor
  algo_fem_decoder_base();
  
  /// Default destructor
  virtual ~algo_fem_decoder_base(){};

  /// Implementation of algo_base::process_word
  virtual bool process_word(PMT::word_t word);

  /// Implementation of algo_base::process_word
  virtual inline PMT::PMT_WORD get_word_class(const PMT::word_t word) const {
    if     ( (word & 0xf000) == 0xf000 )
      return PMT::FEM_HEADER;
    else if( (word & 0xf000) == 0x4000 )
      return PMT::FEM_FIRST_WORD;
    else if( (word & 0xf000) == 0x9000 )
      return PMT::CHANNEL_HEADER;
    else if( (word & 0xf000) == 0xa000 )
      return PMT::CHANNEL_WORD;
    else if( (word & 0xf000) == 0xb000 )
      return PMT::CHANNEL_LAST_WORD;
    else if( (word & 0xf000) == 0xc000 )
      return PMT::FEM_LAST_WORD;
    else
      return PMT::UNDEFINED_WORD;
  }

  /// Implementation of algo_base::check_event_quality
  virtual bool check_event_quality(){return true;};

  /// Override function to reset the instance
  virtual void reset();

  /// A simple round-diff algorithm for binary words
  PMT::word_t round_diff(PMT::word_t ref_id, PMT::word_t subject_id, PMT::word_t diff) const;

protected:

  /// A method to process event header word passed from process_word method
  virtual bool process_event_header(const PMT::word_t word, PMT::word_t &last_word);

  /// A method to decode event header word passed from process_header 
  virtual bool decode_event_header(const PMT::word_t *event_header);

  /**
     TO BE IMPLEMENTED!
     A method to process channel word passed from process_word method.
     Channel word processing must be implemented in the children
  */
  virtual bool process_ch_word(const PMT::word_t word, 
			       PMT::word_t &last_word) = 0;

  /**
     TO BE IMPLEMENTED!
     A method to process 32-bit end-of-fem word.
  */
  virtual bool process_fem_last_word(const PMT::word_t word,
				     PMT::word_t &last_word) = 0;

  /**
     TO BE IMPLEMENTED!
     A method to process 32-bit end-of-event word.
  */
  virtual bool process_event_last_word(const PMT::word_t word,
				       PMT::word_t &last_word) = 0;

  /**
     TO BE IMPLEMENTED!
     A method to handle the case of unexpected event header word while processing channel word.
     This is an operation while processing channel words, hence must be implemented in children.
  */
  virtual bool handle_unexpected_header(const PMT::word_t word, PMT::word_t &last_word)=0;

  /**
     A method to handle the case of unexpected channel word while processing event header.
     Children may override this handling if necessary.
  */
  virtual bool handle_unexpected_ch_word(const PMT::word_t word, PMT::word_t &last_word);
  
  /**
     A method to handle the case of failure in processing event word.
     This class implements such that the stream of input binary words are to be skipped
     until the next event's header is found. Children may override if necessary.
  */
  virtual bool handle_failure_header(const PMT::word_t word, PMT::word_t &last_word);

  /**
     TO BE IMPLEMENTED!
     A method to handle the case of failure in processing channel data word.
     This is called within the channel word processing logic, hence to be implemented in children.
  */
  virtual bool handle_failure_ch_word(const PMT::word_t word, PMT::word_t &last_word)=0;

  //
  // Constants
  //
  // Comment out the following if you deal with an older format with 1 less header
#define INCLUDE_EXTRA_HEADER
#ifdef INCLUDE_EXTRA_HEADER
  static const size_t FEM_HEADER_COUNT=6; ///< Number of event header words
#else
  static const size_t FEM_HEADER_COUNT=5; ///< Number of event header words
#endif

  // variables
  PMT::word_t          _event_header_words[FEM_HEADER_COUNT]; ///< Event header word holder

  //
  // Run control variable
  //
  PROCESS_FLAG  _process;                ///< Process status flag holder
  PMT::word_t   _last_word;              ///< Last word processed
  size_t        _event_header_count;     ///< A counter for event header words
  /// A run control boolean: skips any action till it finds the new event header.
  bool          _search_for_next_event;  
  InfoFEM_t     _header_info;
};

#endif

/** @} */ // end of doxygen group 

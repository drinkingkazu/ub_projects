/**
 * \file constants.hh
 *
 * \ingroup Base
 * 
 * \brief Constants definition header file in this framework
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef CONSTANTS_HH
#define CONSTANTS_HH
#include <string>
#include "types.hh"

/// Defines constants for Message utility
namespace MSG{

  /// Defines message level
  enum Level{
    DEBUG=0,      ///< Message level ... useful to debug a crash
    INFO,         ///< Debug info but not the lowest level
    NORMAL,       ///< Normal stdout
    WARNING,      ///< notify a user in the standard operation mode for an important finding.
    ERROR,        ///< notify a user when something is clearly wrong
    MSG_TYPE_MAX
  };

  const std::string ColorPrefix[MSG_TYPE_MAX] =
    {
      "\033[94m", ///< blue ... DEBUG   
      "\033[92m", ///< green ... INFO
      "\033[95m", ///< magenta ... NORMAL
      "\033[93m", ///< yellow ... WARNING
      "\033[91m"  ///< red ... ERROR
    };
  ///< Color coding of message

  const std::string StringPrefix[MSG_TYPE_MAX] =
    {
      "     [DEBUG]  ", ///< DEBUG message prefix
      "      [INFO]  ", ///< INFO message prefix
      "    [NORMAL]  ", ///< NORMAL message prefix
      "   [WARNING]  ", ///< WARNING message prefix 
      "     [ERROR]  "  ///< ERROR message prefix
    };
  ///< Prefix of message
};

/// Defines file/data-word format type
namespace FORMAT{
  
  /// Defines input file format for decoder (i.e. output of electronics)
  enum INPUT_FILE{
    /// Bin format for decoder (input) and coder (output)   
    BINARY=0,
    /// ASCII format for (de)coder
    ASCII,
    /// Undefined...
    UNDEFINED
  };
};

/// Defines constants for PMT electronics
namespace PMT{
  
  const PMT::ch_number_t INVALID_CH=0xffff;       ///< Invalid (or initialized) channel number
  const PMT::word_t      INVALID_WORD=0xffffffff; ///< Invalid (or initialized) event word
  const PMT::word_t      EMPTY_WORD=0x0;          ///< Empty word

  const PMT::word_t      BEAMGATE_NSAMPLE=1500;   ///< # samples in the beamgate
  const PMT::ch_number_t NUM_PMT_CHANNEL=40;      ///< # of PMT readout channels
  const PMT::ch_adc_t    MAX_ADC=4095;            ///< Maximum ADC value

  /// PMT discriminator type
  enum DISCRIMINATOR {
    BASE_DISC = 0, ///< lowest disc. threshold so-called disc. 0
    COSMIC_DISC,   ///< discriminator for cosmics
    MICHEL_DISC,   ///< discriminator for michel electrons (not used, just a place hlder)
    BEAM_DISC,     ///< discriminator for signal within the beam window
    BEAM_WIN,      ///< discriminator for beam window (forced N samples read-out)
    DISC_MAX
  };

  /// Defines PMT word-type
  enum PMT_WORD{
    UNDEFINED_WORD = 0, ///< Word type ... undefined
    EVENT_FIRST_HEADER, ///< A signal word (no data) to flag the start of headers 
    EVENT_HEADER,       ///< Event header word
    FIRST_WORD,         ///< First word in the event (after the event header, before channel data )
    CHANNEL_HEADER,     ///< Channel header word
    CHANNEL_WORD,       ///< Channel word such as ADC sample
    CHANNEL_LAST_WORD , ///< Last word per channel-wise readout
    FEM_LAST_WORD,      ///< Last word per fem
    EVENT_LAST_WORD     ///< Last word in the event
  };
}

namespace DATA_STRUCT{

  /// Define identifier for a data container class to be loaded through storage_manager::get_data()
  enum DATA_TYPE{
    PMT_WF_COLLECTION=0,        ///< event_waveform ... event-wise collection of waveforms
    TPC_WF_COLLECTION,          ///< tpc_wf_collection ... event-wise collection of tpc waveforms
    TRIG_INFO,                  ///< trig_info ... event-wise trigger information
    PULSE_COLLECTION,           ///< pulse_info_collection ... event-wise collection of reco-ed pulse (generic)
    FIXED_WIN_PULSE_COLLECTION, ///< pulse_info_collection ... output from algo_fixed_window
    THRES_WIN_PULSE_COLLECTION, ///< pulse_info_collection ... output from algo_threshold
    SLIDE_WIN_PULSE_COLLECTION, ///< pulse_info_collection ... output from algo_sliding_window
    USER_COLLECTION,            ///< user_collection ... event-wise collection of user defined variables
    DATA_TYPE_MAX
  };

  /// Define tree name
  const std::string DATA_TREE_NAME[DATA_TYPE_MAX] = {
    "event_waveform",
    "tpc_wf_collection",
    "trig_info",
    "pulse_collection",
    "pulse_fixed_window",
    "pulse_thres_window",
    "pulse_slide_window",
    "user_collection"
  };


}


#endif
/** @} */ // end of doxygen group

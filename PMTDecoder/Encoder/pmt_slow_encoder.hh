/**
 * \file pmt_slow_encoder.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class pmt_slow_encoder
 *
 * @author kazuhiro
 */

/** \addtogroup Analysis

    @{*/

#ifndef PMT_SLOW_ENCODER_HH
#define PMT_SLOW_ENCODER_HH

#include "ana_base.hh"
#include "../FileIO/FileIO-TypeDef.hh"
#include "../Base/Base-TypeDef.hh"

/**
   \class pmt_slow_encoder
   User custom analysis class made by kazuhiro
 */
class pmt_slow_encoder : public ana_base{

public:

  /// Default constructor
  pmt_slow_encoder() : _bin_file() { _name="pmt_slow_encoder"; _fout=0;};

  /// Default destructor
  virtual ~pmt_slow_encoder(){};

  /** IMPLEMENT in Name.cc!
      Initialization method to be called before the analysis event loop.
  */ 
  virtual bool initialize();

  /** IMPLEMENT in Name.cc! 
      Analyze a data event-by-event  
  */
  virtual bool analyze(storage_manager* storage);

  /** IMPLEMENT in Name.cc! 
      Finalize method to be called after all events processed.
  */
  virtual bool finalize();

  void set_mode(bin_io_handler::MODE mode){_bin_file.set_mode(mode);};

  void set_filename(std::string name){_bin_file.set_filename(name);};

  virtual inline PMT::word_t format(FORMAT::PMT_WORD type,PMT::word_t word=0x0) const 
  {
    switch(type){
    case FORMAT::EVENT_FIRST_HEADER:
      word = 0xffffffff;
      break;
    case FORMAT::EVENT_HEADER:
      word = 0xf000 + (0xfff & word);
      break;
    case FORMAT::FIRST_WORD:
      word = 0x4000 + (0xfff & word);
      break;
    case FORMAT::CHANNEL_HEADER:
      word = 0x9000 + (0xfff & word);
      break;
    case FORMAT::CHANNEL_WORD:
      word = 0xa000 + (0xfff & word);
      break;
    case FORMAT::CHANNEL_LAST_WORD:
      word = 0xb000 + (0xfff & word);
      break;
    case FORMAT::FEM_LAST_WORD:
      word = 0xc000;
      break;
    case FORMAT::EVENT_LAST_WORD:
      word = 0xe000;
      break;
    case FORMAT::UNDEFINED_WORD:
      word = 0x0;
    }
    return word;
  };

protected:

  virtual void encode_event_header(const event_waveform* data);
  virtual void encode_ch_word(const event_waveform* data);

  bin_io_handler _bin_file;
  PMT::word_t _words[60000];
  uint16_t    _index;

  
};

#endif

/** @} */ // end of doxygen group 

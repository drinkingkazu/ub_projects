/**
 * \file decoder_manager.hh
 *
 * \ingroup Decoder
 * 
 * \brief Main decoder task manager class def 
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Decoder

    @{*/

#ifndef DECODER_MANAGER_HH
#define DECODER_MANAGER_HH

#include <stdint.h>
#include <algorithm>
#include <iterator>
#include <vector>
#include <time.h>

#include "storage_manager.hh"
#include "bin_io_handler.hh"
#include "algo_base.hh"
#include "ana_base.hh"

/**
  \class decoder_manager
  A handler class for decoding task. It operates (1) binary/ascii file
  I/O, (2) decoding of data word, (3) creation & maintaining of data
  storage manager. 
 */
class decoder_manager : public decoder_base {

public:

  /// Default constructor
  decoder_manager();

  /// Default destructor
  virtual ~decoder_manager(){};

  /// Setter for the input filename
  void add_input_filename(std::string name){ _fin.set_filename(name);};

  /// Setter for the output filename
  void set_output_filename(std::string name){ _storage->set_out_filename(name);};

  void set_data_to_write(PMT::DATA_TYPE type,bool write=true)
  {_storage->set_data_to_write(type,write);};

  /// Setter for the input file format
  void set_format(FORMAT::INPUT_FILE fmt){_fin.set_format(fmt);};

  /// Setter for the decoding algorithm
  void set_decoder(algo_base* algo){_decoder=algo;};

  /// Setter for the debug mode
  void debug_mode(bool debug){_debug_mode=debug;};

  /// A method to start a batch decoding process
  bool run();

  /// A method to initialize members for new decoding process.
  void reset();
  
private:

  /// A method to be called before run() to initialize variables
  bool initialize();

  /// A method 
  bool decode();

  /// A method to finalize decoding process, to be called after all is done.
  bool finalize();

  /// A method to open input/output files.
  bool open_file();

  //
  // Run control variables
  //
  bool _debug_mode;   ///< Debug mode boolean
  
  std::vector<ana_base*> _analyzers; ///< A collection of analyzers to be run

  //
  // File IO variables
  //
  FORMAT::INPUT_FILE _format; ///< Input file format type
  std::string _input_file;    ///< Input file name
  bin_io_handler _fin;        ///< Input file I/O handler

  // storage I/O
  storage_manager* _storage;  ///< Output file I/O handler
  algo_base* _decoder;        ///< Decoding algorithm

};

#endif
/** @} */ // end of doxygen group 

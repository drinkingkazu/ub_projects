/**
 * \file storage_manager.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Higher elve data handling class def header file.
 *
 * @author Kazu - Nevis 2013
 */
/** \addtogroup DataFormat

    @{*/

#ifndef STORAGE_MANAGER_HH
#define STORAGE_MANAGER_HH

#include <TFile.h>
#include <TChain.h>

#include "decoder_base.hh"
#include "event_waveform.hh"
#include "trig_info.hh"
#include "pulse_info.hh"
#include "user_info.hh"

/**
   \class storage_manager
   A higher level manager class to handle event-wise data and output file.
*/
class storage_manager : public decoder_base {

public:

  /// Defines I/O mode ... currently only read OR write.
  enum MODE{
    READ,       ///< I/O mode: READ
    WRITE,      ///< I/O mode: WRITE
    BOTH,       ///< I/O mode both read & write
    UNDEFINED   ///< I/O mode: undefined ...
  };

  /// Defines I/O process status flags
  enum STATUS{
    INIT,       ///< I/O operation status: initiated
    OPENED,     ///< I/O file opened
    READY_IO,   ///< All pre-I/O process finished (making TTrees, etc.)
    OPERATING,  ///< Reading/Writing events
    CLOSED      ///< I/O finished and file closed already
  };

  /// Default constructor. Allow user to specify the I/O mode here.
  storage_manager(MODE mode=UNDEFINED);

  /// Default destructor.
  virtual ~storage_manager(){};

  /// Setter to specify a certain data class to be read from input
  void set_data_to_read(DATA_STRUCT::DATA_TYPE type,bool read=true)
  {_read_data_array[type]=read;};

  /// Setter to specify a certain data class to be written in the output.
  void set_data_to_write(DATA_STRUCT::DATA_TYPE type,bool write=true)
  {_write_data_array[type]=write;};

  /// Setter for I/O mode.
  void set_io_mode(MODE mode)         {_mode=mode;};

  /// Setter for input filename
  //void set_in_filename(std::string name) {_in_fname=name;};
  void add_in_filename(std::string name) {_in_fnames.push_back(name);};

  /// Setter for output filename
  void set_out_filename(std::string name) {_out_fname=name;};

  /// Getter for I/O mode.
  MODE io_mode() const         {return _mode;};

  /// Getter for an array of input files set by the user
  std::vector<std::string> input_filename() const {return _in_fnames;};

  /// Getter for I/O file name.
  std::string output_filename() const {return _out_fname;};

  /// Getter for process status stamp. See storage_manager::STATUS
  STATUS status() const        {return _status;};

  /// Reset variables and I/O process to prepare for a new I/O.
  void reset(size_t len=0);

  /// Open I/O file. Return true upon success.
  bool open();  

  /// Utility boolean to check if file is opened.
  bool is_open();

  /// Utility boolean to check if file is opened and ready for I/O
  bool is_ready_io();

  /// Stores an event to the output in WRITE mode, and loads an event to local memory in READ mode.
  bool next_event(); 

  /// Move to the TTree entry with the provided index number
  bool go_to(uint32_t index);

  /// Close I/O file. 
  bool close(); 

  /** Universal data pointer getter to return data_base* pointer for specified data type.
      A user then cast the pointer type to whatever the relevant data class pointer type to access members.
   */
  data_base* get_data(DATA_STRUCT::DATA_TYPE type);

  /*
  /// Return a const event_waveform for READ-ONLY
  const event_waveform get_event_waveform() const 
  {return *((event_waveform*)(_ptr_data_array[DATA_STRUCT::WF_COLLECTION]));};

  /// Return a const trig_info for READ-ONLY
  const trig_info get_trig_info() const  
  {return *((trig_info*)(_ptr_data_array[DATA_STRUCT::TRIG_INFO]));};

  /// Return a const pulse_collection for READ-ONLY
  const pulse_collection get_pulse_collection() const 
  { return *((pulse_collection*)(_ptr_data_array[DATA_STRUCT::PULSE_COLLECTION]));};

  /// Return a pointer to event_waveform for READ & WRITE
  event_waveform* get_event_waveform_writeable()  
  {return (event_waveform*)(_ptr_data_array[DATA_STRUCT::WF_COLLECTION]);};

  /// Retrun a pointer to trig_info for READ & WRITE
  trig_info* get_trig_info_writeable()   
  {return (trig_info*)(_ptr_data_array[DATA_STRUCT::TRIG_INFO]);};

  /// Return a pointer to pulse_collection for READ & WRITE
  pulse_collection* get_pulse_collection_writeable() 
  {return (pulse_collection*)(_ptr_data_array[DATA_STRUCT::PULSE_COLLECTION]);};
  */

  /// Getter for a shared object instance pointer. Not limited to be a singleton.
  static storage_manager* get() 
  { if(!me) me= new storage_manager(); return me; };
  
  /// killer for a shared object instance.
  static void kill() 
  { if(me) delete me;};

  /// Getter for a number of events in the file.
  inline uint32_t get_entries() const {return _nevents;};

  /// Getter for a TTree index
  inline uint32_t get_index()   const {return _index;};

  /// Getter for a counter of read-out events
  inline uint32_t get_entries_read() const {return _nevents_read;};

  /// Getter for a counter of written-out events
  inline uint32_t get_entires_written() const {return _nevents_written;};
  
private:

  static storage_manager* me; ///< shared object instance pointer

  /// 
  void create_data_ptr(DATA_STRUCT::DATA_TYPE type);

  void delete_data_ptr(DATA_STRUCT::DATA_TYPE type);

  /// Load next event from imput  
  bool read_event();
  
  /// Write the current event into output
  bool write_event();

  /// Called upon opening a file to prepare read/write events in TTree.
  bool prepare_tree();

  /// tree index marker
  uint32_t _index;

  /// number of events read/written
  uint32_t _nevents, _nevents_read, _nevents_written;

  /// status control stamp
  STATUS _status;

  /// I/O mode 
  MODE _mode;

  /// Boolean to record what data to be read out from a file
  bool _read_data_array[DATA_STRUCT::DATA_TYPE_MAX];

  /// Boolean to record what data to be written out from a file
  bool _write_data_array[DATA_STRUCT::DATA_TYPE_MAX];

  /// Data pointer array
  data_base *_ptr_data_array[DATA_STRUCT::DATA_TYPE_MAX];

  /// I/O filename
  std::string _out_fname;
  std::vector<std::string> _in_fnames;

  /// TTree name
  const std::string _treename;

  /// ROOT file instance
  //TFile *_fin, *_fout;
  TFile *_fout;

  /// TTree instance
  //TTree *_in_tree, *_out_tree;  
  TChain *_in_ch[DATA_STRUCT::DATA_TYPE_MAX];
  TTree  *_out_ch[DATA_STRUCT::DATA_TYPE_MAX];

};

#endif
/** @} */ // end of doxygen group storage_manager

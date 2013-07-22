/**
 * \file trig_info.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for trig_info 
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/
#ifndef TRIG_INFO_HH
#define TRIG_INFO_HH

#include "data_base.hh"

/**
   \class trig_info
   A class to hold trigger module output data
 */
class trig_info : public data_base {

public:

  /// Default constructor
  trig_info() : data_base() { init_vars(); };

  /// Default copy constructor to avoid memory leak in ROOT streamer
  trig_info(const trig_info &original);

  /// Default destructor
  virtual ~trig_info(){};

  /// Setter for 16 MHz reminder
  void set_reminder_16MHz(uint16_t remin){_reminder_16MHz=remin;};

  /// Setter for 64 MHz reminder
  void set_reminder_64MHz(uint64_t remin){_reminder_64MHz=remin;};

  /// Setter for frame id
  void set_trig_frame_id(PMT::word_t id){_trig_frame_id=id;};

  /// Setter for sample id
  void set_trig_timeslice(PMT::word_t t){_trig_timeslice=t;};

  /// Setter for trigger ID
  void set_trig_id(uint16_t id){_trig_id=id;};

  /// Setter for PMT trigger data
  void set_pmt_data(uint16_t data){_pmt_data=data;};

  /// Getter for 16 MHz reminder
  uint16_t reminder_16MHz() {return _reminder_16MHz;};

  /// Getter for 64 MHz reminder
  uint16_t reminder_64MHz() {return _reminder_64MHz;};

  /// Getter for frame id
  PMT::word_t trig_frame_id(){return _trig_frame_id;};

  /// Getter for sample id
  PMT::word_t trig_timeslice(){return _trig_timeslice;};

  /// Getter for trigger ID
  uint16_t trig_id(){return _trig_id;};

  /// Getter for PMT trigger data
  uint16_t pmt_data(){return _pmt_data;};

  /// Setter for trigger bits
  void set_trigger_bits(bool trig_pc,  bool trig_ext,
			bool active,   bool gate1_in,
			bool gate2_in, bool veto_in,
			bool calib)
  { _trig_pc  = trig_pc;   _trig_ext = trig_ext;
    _active   = active;    _gate1_in = gate1_in;
    _gate2_in = gate2_in; _veto_in  = veto_in;
    _calib    = calib;
  };

  /// Getter for trigger bit
  bool trig_pc(){return _trig_pc;};

  /// Getter for trigger bit
  bool trig_ext(){return _trig_ext;};

  /// Getter for trigger bit
  bool active(){return _active;};

  /// Getter for trigger bit
  bool gate1(){return _gate1_in;};

  /// Getter for trigger bit
  bool gate2(){return _gate2_in;};

  /// Getter for trigger bit
  bool veto_in(){return _veto_in;};

  /// Getter for trigger big
  bool calib(){return _calib;};

  /// Clear method override
  virtual void clear_data();

protected:

  void init_vars();

  uint16_t    _trig_timeslice;
  PMT::word_t _trig_frame_id;
  PMT::word_t _trig_id;
  uint16_t    _pmt_data;
  bool        _trig_pc;
  bool        _trig_ext;
  bool        _active;
  bool        _gate1_in;
  bool        _gate2_in;
  bool        _veto_in;
  bool        _calib;
  uint16_t    _reminder_64MHz;
  uint16_t    _reminder_16MHz;

  ////////////////////////
  ClassDef(trig_info,3)
  ////////////////////////

};

#endif
/** @} */ // end of doxygen group 

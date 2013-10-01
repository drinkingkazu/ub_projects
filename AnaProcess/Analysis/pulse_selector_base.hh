/**
 * \file pulse_selector_base.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class pulse_selector_base
 *
 * @author kazuhiro
 */

/** \addtogroup Analysis

    @{*/

#ifndef PULSE_SELECTOR_BASE_HH
#define PULSE_SELECTOR_BASE_HH

#include "ana_base.hh"

/**
   \class pulse_selector_base
   This is an abstract base class for pulse selection alnalysis code.
   Children class should implement the actual selection criteria.
   analyze() method should retrieve data an store selection outcome.
   select_pusle() method takes pulse_info* data pointer, and returns true/false
   that indicates selected/rejected decision flag. 
 */
class pulse_selector_base : public ana_base{

public:

  /// Default constructor
  pulse_selector_base();

  /// Default destructor
  virtual ~pulse_selector_base(){};

  /**
     Selection method to indicate whether the given pulse should be selected or not
   */
  virtual bool select_pulse(const pulse_info* data)=0;

  /**
     Config method to specify which type of pulse information should be used
   */
  void set_pulse_type(DATA_STRUCT::DATA_TYPE type);

  /**
     Getter for the specified pulse type to be used
   */
  DATA_STRUCT::DATA_TYPE pulse_type() const {return _pulse_type;};

protected:

  DATA_STRUCT::DATA_TYPE _pulse_type;
};

#endif

/** @} */ // end of doxygen group 

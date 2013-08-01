/**
 * \file decoder_base.hh
 *
 * \ingroup Base
 * 
 * \brief Class definition file of decoder_base
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef DECODER_BASE_HH
#define DECODER_BASE_HH

#include "types.hh"
#include "constants.hh"
#include "messenger.hh"

/**
 \class decoder_base
 Very base class of all everything else defined in this framework.
*/
class decoder_base {

public:

  /// Default constructor
  decoder_base();
  
  /// Default copy constructor
  decoder_base(const decoder_base &original) : _name(original._name){};

  /// Default destructor
  virtual ~decoder_base(){};

  /// Setter for the verbosity level 
  void set_verbosity(MSG::Level level);

  /// Getter for the verbosity level
  MSG::Level get_verbosity() const {return _verbosity_level;};

  /// Getter for the class name
  const std::string class_name() const {return _name;};

protected:

  char _buf[200];                           ///< char buffer for message manipulation
  bool       _verbosity[MSG::MSG_TYPE_MAX]; ///< holder for enabled message levels
  MSG::Level _verbosity_level;              ///< holder for specified verbosity level
  std::string _name;                        ///< class name holder

};

#endif

/** @} */ // end of doxygen group

/**
 * \file data_base.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Base class def for data class
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/
#ifndef DATA_BASE_HH
#define DATA_BASE_HH

#include <vector>
#include <limits>
#include <TObject.h>

#include "decoder_base.hh"

/**
   \class data_base
   A base coass for PMT-wise & event-wise data holder class
 */
class data_base : public TObject {

public:

  /// Default constructor
  data_base() : TObject(){};

  /// Default copy constructor to avoid memory leak in ROOT streamer
  data_base(const data_base &original) 
    : TObject(original) 
  {};

  /// Default destructor
  virtual ~data_base(){};

  /// Clear method
  virtual void clear_data(){};

  ////////////////////////
  ClassDef(data_base,2)
  ////////////////////////

};

#endif
/** @} */ // end of doxygen group 

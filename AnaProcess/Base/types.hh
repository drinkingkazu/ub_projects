/**
 * \file types.hh
 *
 * \ingroup Base
 * 
 * \brief Typedefs header file for this framework
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef TYPES_HH
#define TYPES_HH

#include <stdint.h>
#include <vector>

/// Defines some data types for this framework
namespace PMT{
  typedef uint32_t semapho_t;      ///< Used for semapho
  typedef uint32_t word_t;         ///< Used as a data word format for the PMT electronics
  typedef uint16_t timeslice_t;    ///< Used as a unit of digitized time
  typedef uint16_t ch_adc_t;       ///< Used as a unit of digitized ADC count
  typedef uint16_t ch_number_t;    ///< Channel number type definition
  typedef std::vector<ch_adc_t> ch_waveform_t; ///< Channel waveform type definition

};

#endif
/** @} */ // end of doxygen group

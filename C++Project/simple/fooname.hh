// This example namespace header is commented in doxygen style.

/**
 * \file fooname.hh
 *
 * \ingroup simple
 * 
 * \brief Defining namespace FOO_AM_I
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup FOO_AM_I

    @{*/

#ifndef FOONAME_HH
#define FOONAME_HH

/**
   \namespace FOO_AM_I
   A simple namespace example
 */
namespace FOO_AM_I{

  /// enum as one of popular namespace element
  enum FOO_NAMES{
    /// Beginning of everything (=0) is my first name.
    KAZU=0, 
    /// End of everything is my last name.
    TERAO   
  };

};

#endif

/** @} */ // end of doxygen group foo

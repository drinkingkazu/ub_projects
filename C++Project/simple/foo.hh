// This example class header is commented in doxygen style.

/**
 * \file foo.hh
 *
 * \ingroup simple
 * 
 * \brief Class def header for Mr. foo
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup foo

    @{*/

#ifndef FOO_HH
#define FOO_HH

#include <iostream>

/**
 * \class foo
 * 
 * \brief Example for a super simple class definition, doctor foo.
 */
class foo{

public:

  /// Default constructor of Mr. Foo
  foo(){};   

  /// Default destructor of Mr. Foo. Why virtual? No specific reason...
  virtual ~foo(){};  

  /// One and only one example member function of Mr. Foo
  void speak();   

};

#endif

/** @} */ // end of doxygen group foo

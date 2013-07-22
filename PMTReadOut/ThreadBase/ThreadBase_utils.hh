#ifndef _UTILS__
#define _UTILS__

#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>

inline void split (std::string text, std::string separators,
            std::vector<std::string,
	    std::allocator<std::string> >& words) {
  
  size_t n     = text.length ();
  size_t start = text.find_first_not_of (separators);
  
  while (start < n) {
    size_t stop = text.find_first_of (separators, start);
    if (stop > n) stop = n;
    words.push_back (text.substr (start, stop-start));
    start = text.find_first_not_of (separators, stop+1);
  }
}
   
 
inline int int_from_string(std::string s)
  {
    std::istringstream istr(s.c_str());
    long int q;
    istr >> q;
    return q;
  }

template <class T> 
inline std::string to_string(T i)
{
  std::ostringstream ostr;
  ostr << i ;      
  return ostr.str();
}

#endif

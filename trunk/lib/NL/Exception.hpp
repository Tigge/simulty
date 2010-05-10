#ifndef _EXCEPTION_HPP_
#define _EXCEPTION_HPP_

//#include "NL.hpp"

#include <iostream>
#include <exception>

namespace NL {

    class Exception: public std::exception {
          
      virtual const char* what() const throw();
          
    };
    
}


#endif

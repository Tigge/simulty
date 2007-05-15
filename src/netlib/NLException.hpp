#ifndef _NEXCEPTION_H_
#define _NEXCEPTION_H_

#include "NL.hpp"

#include <iostream>
#include <exception>


class NException: public std::exception {
      
  virtual const char* what() const throw();
      
};


#endif

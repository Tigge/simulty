#ifndef _NEXCEPTION_H_
#define _NEXCEPTION_H_

#include "netlib.h"

#include <iostream>
#include <exception>


class NException: public std::exception {
      
  virtual const char* what() const throw();
      
};


#endif

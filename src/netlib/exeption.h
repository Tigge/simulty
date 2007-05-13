#ifndef _NEXCEPTION_H_
#define _NEXCEPTION_H_

#include "netlib.h"

#include <iostream>
#include <exception>

using namespace std;

class NException: public exception {
      
  virtual const char* what() const throw();
      
};


#endif

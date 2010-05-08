#include <iostream>
#include <exception>

#include "NLException.hpp"


  const char* NLException::what() const throw() {
    return "General NL error occured";    
  }




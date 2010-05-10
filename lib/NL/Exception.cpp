#include <iostream>
#include <exception>

#include "Exception.hpp"

namespace NL {

    const char* Exception::what() const throw() {
        return "General NL error occured";    
    }

}



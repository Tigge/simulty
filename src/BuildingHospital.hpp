#ifndef _BUILDINGHOSPITAL_HPP_
#define _BUILDINGHOSPITAL_HPP_

#include "Building.hpp"

class BuildingHospital: public Building {

    public:

    BuildingHospital(Point position, int owner);
    
    virtual unsigned char getType();
    virtual unsigned char getWidth();
    virtual unsigned char getHeight();    
  // Associations
  // Attributes
  // Operations
};

#endif

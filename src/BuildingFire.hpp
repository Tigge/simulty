#ifndef _BUILDINGFIRE_HPP_
#define _BUILDINGFIRE_HPP_

#include "Building.hpp"

class BuildingFire: public Building {

    public:

    BuildingFire(Point position, int owner, Date built);
    
    virtual unsigned char getType();
    virtual unsigned char getWidth();
    virtual unsigned char getHeight();    
  // Associations
  // Attributes
  // Operations
};

#endif

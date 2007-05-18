#ifndef _BUILDING_POLICE_HPP_
#define _BUILDING_POLICE_HPP_

#include "Building.hpp"

class BuildingPolice: public Building {

    public:

    BuildingPolice(Point position, int owner);
    
    virtual unsigned char getType();
    virtual unsigned char getWidth();
    virtual unsigned char getHeight();    
  // Associations
  // Attributes
  // Operations
};

#endif

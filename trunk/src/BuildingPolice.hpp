#ifndef _BUILDINGPOLICE_HPP_
#define _BUILDINGPOLICE_HPP_

#include "Building.hpp"

class BuildingPolice: public Building {

    public:

    BuildingPolice(Point position, int owner, Date built);

    virtual unsigned char getType();
    virtual unsigned char getWidth();
    virtual unsigned char getHeight();
  // Associations
  // Attributes
  // Operations
};

#endif

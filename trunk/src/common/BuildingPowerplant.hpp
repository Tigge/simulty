#ifndef _BUILDINGPOWERPLANT_HPP_
#define _BUILDINGPOWERPLANT_HPP_

#include "Building.hpp"

class BuildingPowerplant: public Building {

    public:

    BuildingPowerplant(Point position, int owner, Date built);

    virtual unsigned char getType();
    virtual unsigned char getWidth();
    virtual unsigned char getHeight();
};

#endif

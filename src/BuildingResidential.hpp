#ifndef _BUILDINGRESIDENTIAL_HPP_
#define _BUILDINGRESIDENTIAL_HPP_

#include "Building.hpp"

class BuildingResidential : public Building {
  private:
    unsigned int level, style, time;
  public:
    BuildingResidential(Point position, int owner);

    virtual unsigned char getType();

};

#endif

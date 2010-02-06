#ifndef _BUILDINGCOMMERSIAL_HPP_
#define _BUILDINGCOMMERSIAL_HPP_

#include "BuildingZone.hpp"

class BuildingCommersial : public BuildingZone {
  private:

  public:
    BuildingCommersial(Point position, unsigned char owner, int w, int h, 
        Date built, int level, int style);


    virtual unsigned char getType();

};

#endif

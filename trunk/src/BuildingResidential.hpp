#ifndef _BUILDINGRESIDENTIAL_HPP_
#define _BUILDINGRESIDENTIAL_HPP_

#include "BuildingZone.hpp"

class BuildingResidential : public BuildingZone {
  private:

  public:
    BuildingResidential(Point position, unsigned char owner, int w, int h, 
        Date built, int level, int style);


    virtual unsigned char getType();

};

#endif

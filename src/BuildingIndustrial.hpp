#ifndef _BUILDINGINDUSTRIAL_HPP_
#define _BUILDINGINDUSTRIAL_HPP_

#include "BuildingZone.hpp"

class BuildingIndustrial : public BuildingZone {
  private:

  public:
    BuildingIndustrial(Point position, unsigned char owner, int w, int h, 
        Date built, int level, int style);


    virtual unsigned char getType();

};

#endif

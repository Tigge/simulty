#ifndef _BUILDINGZONE_HPP_
#define _BUILDINGZONE_HPP_

#include "Building.hpp"
#include "Date.hpp"

class BuildingZone : public Building {
  private:
    unsigned int level;
    unsigned int style;

  public:
    BuildingZone(Point position, unsigned char  owner, int w, int h,
        Date built, int level, int style);

    virtual unsigned char getType() = 0;

    unsigned int getLevel();
    unsigned int getStyle();

    unsigned int getInhabitants();
    unsigned int getJobs();
    unsigned int getStores();
};

#endif

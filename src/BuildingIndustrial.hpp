#ifndef _BUILDINGINDUSTRIAL_HPP_
#define _BUILDINGINDUSTRIAL_HPP_

#include "Building.hpp"

class BuildingIndustrial : public Building {
  private:
    unsigned int level, style, time;
  public:
    BuildingIndustrial(Point position, int owner);

    virtual unsigned char getType();

};

#endif

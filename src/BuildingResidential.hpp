#ifndef _BUILDINGRESIDENTIAL_HPP_
#define _BUILDINGRESIDENTIAL_HPP_

#include "Building.hpp"

class BuildingResidential : public Building {
  private:
    unsigned int level, style, time;
  public:
    BuildingResidential(Point position, int owner);

    virtual unsigned char getType();
    virtual unsigned char getWidth();
    virtual unsigned char getHeight();

    virtual void setWidth(unsigned char set_width);
    virtual void setHeight(unsigned char set_height);
};

#endif

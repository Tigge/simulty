#ifndef _BUILDINGCOMMERSIAL_HPP_
#define _BUILDINGCOMMERSIAL_HPP_

#include "Building.hpp"

class BuildingCommersial : public Building {
  private:
    unsigned int level, style, time;
  public:
    BuildingCommersial(Point position, int owner);

    virtual unsigned char getType();
    virtual unsigned char getWidth();
    virtual unsigned char getHeight();

    virtual void setWidth(unsigned char set_width);
    virtual void setHeight(unsigned char set_height);
};

#endif

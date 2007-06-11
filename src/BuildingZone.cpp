#include "BuildingZone.hpp"

BuildingZone::BuildingZone(Point position, unsigned char  owner, int w, int h,
    Date built, int level, int style)
    : Building(position, owner, w, h, built) {
  
  this->level = level;
  this->style = style;
  
}

unsigned int BuildingZone::getLevel() {
  return this->level;
}
unsigned int BuildingZone::getStyle() {
  return this->style;
}

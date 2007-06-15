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

unsigned int BuildingZone::getInhabitants() {
  if(getType() != Building::TYPE_RESIDENTIAL)
    return 0;

  // We take for granted that no households of more than 1 person is living in a shed. move *level inside pow?
  return (unsigned int)(pow(width*height, 2.0)) * level;
}

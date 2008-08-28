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

  // We take for granted that no households of more than 4 person is living in 
  // a small villa (size 1). TODO: this should depend on thrive.
  return (unsigned int)(pow(width * height * level, 2.0)) * 4 ;
}

unsigned int BuildingZone::getJobs() {
  if(getType() != Building::TYPE_INDUSTRIAL)
    return 0;
  
  return (unsigned int)(pow(width * height * level, 2.0)) * 4;
}


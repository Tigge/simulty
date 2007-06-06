#include "BuildingIndustrial.hpp"

BuildingIndustrial::BuildingIndustrial(Point position, int owner)
    : Building(position, owner) {

  width = 1;
  height = 1;
}


unsigned char BuildingIndustrial::getType() {
    return Building::TYPE_INDUSTRIAL;
}



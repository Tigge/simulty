#include "BuildingResidential.hpp"

BuildingResidential::BuildingResidential(Point position, int owner)
    : Building(position, owner) {

  width = 1;
  height = 1;
}


unsigned char BuildingResidential::getType() {
    return Building::TYPE_RESIDENTIAL;
}


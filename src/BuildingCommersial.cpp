#include "BuildingCommersial.hpp"

BuildingCommersial::BuildingCommersial(Point position, int owner)
    : Building(position, owner) {

  width = 1;
  height = 1;
}


unsigned char BuildingCommersial::getType() {
    return Building::TYPE_COMMERSIAL;
}


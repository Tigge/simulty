#include "BuildingResidential.hpp"

BuildingResidential::BuildingResidential(Point position, int owner)
    : Building(position, owner) {

  width = 1;
  height = 1;
}


unsigned char BuildingResidential::getType() {
    return Building::TYPE_RESIDENTIAL;
}

unsigned char BuildingResidential::getWidth() {
    return width;
}
unsigned char BuildingResidential::getHeight() {
    return height;
}

void BuildingResidential::setWidth(unsigned char set_width) {
  width = set_width;
}

void BuildingResidential::setHeight(unsigned char set_height) {
  height = set_height;
}

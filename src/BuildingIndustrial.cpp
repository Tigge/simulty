#include "BuildingIndustrial.hpp"

BuildingIndustrial::BuildingIndustrial(Point position, int owner)
    : Building(position, owner) {

  width = 1;
  height = 1;
}


unsigned char BuildingIndustrial::getType() {
    return Building::TYPE_RESIDENTIAL;
}

unsigned char BuildingIndustrial::getWidth() {
    return width;
}
unsigned char BuildingIndustrial::getHeight() {
    return height;
}

void BuildingIndustrial::setWidth(unsigned char set_width) {
  width = set_width;
}

void BuildingIndustrial::setHeight(unsigned char set_height) {
  height = set_height;
}

#include "BuildingCommersial.hpp"

BuildingCommersial::BuildingCommersial(Point position, int owner)
    : Building(position, owner) {

  width = 1;
  height = 1;
}


unsigned char BuildingCommersial::getType() {
    return Building::TYPE_RESIDENTIAL;
}

unsigned char BuildingCommersial::getWidth() {
    return width;
}
unsigned char BuildingCommersial::getHeight() {
    return height;
}

void BuildingCommersial::setWidth(unsigned char set_width) {
  width = set_width;
}

void BuildingCommersial::setHeight(unsigned char set_height) {
  height = set_height;
}

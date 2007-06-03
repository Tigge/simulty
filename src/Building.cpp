#include "Building.hpp"

#include <iostream>

Building::Building(Point position, int owner) {

    this->position = position;
    this->owner    = owner;

    std::cerr << "Pos is: " << getPosition() << std::endl;
}



Building::~Building (  ){
}

Point Building::getPosition() {
    return this->position;
}

unsigned char Building::getWidth() {
    return width;
}
unsigned char Building::getHeight() {
    return height;
}

void Building::setWidth(unsigned char set_width) {
  width = set_width;
}
void Building::setHeight(unsigned char set_height) {
  height = set_height;
}

void Building::render (  ){
}

void Building::update (  ){
}


int Building::getZ() {

  int base = getPosition().getX() + getPosition().getY();
  if(getWidth() > getHeight())
    return base - 1 + getWidth();
  else
    return base - 1 + getHeight();
}
int Building::getOwner() {
    return owner;
}

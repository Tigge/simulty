#include "Building.hpp"

#include <iostream>

Building::Building(Point position, unsigned char owner, int w, int h, Date built) {

  this->position = position;
  this->owner    = owner;

  this->built    = built;

  setWidth(w);
  setHeight(h);
  //std::cerr << "Pos is: " << getPosition() << std::endl;
}


Building::~Building(){

}

Point Building::getPosition() {
  return position;
}

unsigned char Building::getWidth() {
  return width;
}
unsigned char Building::getHeight() {
  return height;
}

void Building::setWidth(unsigned char width) {
  this->width = width;
}
void Building::setHeight(unsigned char height) {
  this->height = height;
}

void Building::render(){
}

void Building::update(){
}


unsigned int Building::getZ() {

  unsigned int base = getPosition().getX() + getPosition().getY();
  if(getWidth() > getHeight())
    return base - 1 + getWidth();
  else
    return base - 1 + getHeight();
}
int Building::getOwner() {
  return owner;
}

Date Building::getBuildDate() {
  return built;
}


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

void Building::render (  ){
}

void Building::update (  ){
}


int Building::getZ() {
    return getPosition().getX() + getPosition().getY();
}
int Building::getOwner() {
    return owner;
}

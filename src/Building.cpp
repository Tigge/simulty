#include "Building.hpp"

#include <iostream>

Building::Building(Point position) {

    this->position = position;

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

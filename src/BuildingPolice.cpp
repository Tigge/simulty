#include "BuildingPolice.hpp"

BuildingPolice::BuildingPolice(Point position, int owner) 
    : Building(position, owner) {

    width  = 3;
    height = 3;
}


unsigned char BuildingPolice::getType() {
    return Building::TYPE_POLICE;
}

unsigned char BuildingPolice::getWidth() {
    return 3;
}
unsigned char BuildingPolice::getHeight() {
    return 3;
}

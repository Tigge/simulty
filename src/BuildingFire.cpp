#include "BuildingPolice.hpp"

BuildingFire::BuildingFire(Point position, int owner) 
    : Building(position, owner) {

    width  = 3;
    height = 3;
}


unsigned char BuildingFire::getType() {
    return Building::TYPE_FIRE;
}

unsigned char BuildingFire::getWidth() {
    return 3;
}
unsigned char BuildingFire::getHeight() {
    return 3;
}

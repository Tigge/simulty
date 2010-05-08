#include "BuildingFire.hpp"

BuildingFire::BuildingFire(Point position, int owner, Date built) 
    : Building(position, owner, 3, 3, built) {

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

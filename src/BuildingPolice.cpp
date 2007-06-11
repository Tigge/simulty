#include "BuildingPolice.hpp"

BuildingPolice::BuildingPolice(Point position, int owner, Date built) 
    : Building(position, owner, 3, 3, built) {

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

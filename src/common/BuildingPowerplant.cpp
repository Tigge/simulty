#include "BuildingPowerplant.hpp"

BuildingPowerplant::BuildingPowerplant(Point position, int owner, Date built) 
    : Building(position, owner, 3, 3, built) {

}

unsigned char BuildingPowerplant::getType() {
    return Building::TYPE_POWERPLANT;
}

unsigned char BuildingPowerplant::getWidth() {
    return 3;
}
unsigned char BuildingPowerplant::getHeight() {
    return 3;
}

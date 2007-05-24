#include "BuildingHospital.hpp"

BuildingHospital::BuildingHospital(Point position, int owner) 
    : Building(position, owner) {

    width  = 3;
    height = 3;
}


unsigned char BuildingHospital::getType() {
    return Building::TYPE_HOSPITAL;
}

unsigned char BuildingHospital::getWidth() {
    return 3;
}
unsigned char BuildingHospital::getHeight() {
    return 3;
}

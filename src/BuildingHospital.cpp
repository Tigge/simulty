#include "BuildingHospital.hpp"

BuildingHospital::BuildingHospital(Point position, int owner, Date built) 
    : Building(position, owner, 3, 3, built) {

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

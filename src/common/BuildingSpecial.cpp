#include "BuildingSpecial.hpp"

BuildingSpecial::BuildingSpecial(BuildingInformation *bi, Point position,
        unsigned char owner, Date built): Building(position, 
        owner, bi->getWidth(), bi->getHeight(), built) {

    this->bi = bi;
}


BuildingSpecial::~BuildingSpecial(){
    
}

unsigned char BuildingSpecial::getType() {
    return this->bi->getType();
}

BuildingInformation* BuildingSpecial::getBuildingInformation() {
    return this->bi;
}

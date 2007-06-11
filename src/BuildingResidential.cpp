#include "BuildingResidential.hpp"

BuildingResidential::BuildingResidential(Point position, unsigned char owner,
    int w, int h, Date built, int level, int style)
    : BuildingZone(position, owner, w, h, built, level, style) {

}


unsigned char BuildingResidential::getType() {
    return Building::TYPE_RESIDENTIAL;
}


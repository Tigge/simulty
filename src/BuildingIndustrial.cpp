#include "BuildingIndustrial.hpp"

BuildingIndustrial::BuildingIndustrial(Point position, unsigned char owner,
    int w, int h, Date built, int level, int style)
    : BuildingZone(position, owner, w, h, built, level, style) {

}


unsigned char BuildingIndustrial::getType() {
    return Building::TYPE_INDUSTRIAL;
}



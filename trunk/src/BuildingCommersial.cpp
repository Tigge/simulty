#include "BuildingCommersial.hpp"

BuildingCommersial::BuildingCommersial(Point position, unsigned char owner,
    int w, int h, Date built, int level, int style)
    : BuildingZone(position, owner, w, h, built, level, style) {

}


unsigned char BuildingCommersial::getType() {
    return Building::TYPE_COMMERSIAL;
}



#ifndef _BUILDING_FACTORY_HPP_
#define _BUILDING_FACTORY_HPP_

#include <vector>

#include "Point.hpp"

#include "Map.hpp"

#include "Building.hpp"


class BuildingFactory {

    public:

    static Building *getBuilding(int buildingType, Point position, int owner);

};

#endif

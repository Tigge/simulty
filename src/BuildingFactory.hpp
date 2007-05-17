
#ifndef _BUILDING_FACTORY_HPP_
#define _BUILDING_FACTORY_HPP_

#include <vector>
#include "Map.hpp"
#include "Building.hpp"


class BuildingFactory {

    public:

    static Building *getBuilding(int buildingType, int x, int y);

};

#endif

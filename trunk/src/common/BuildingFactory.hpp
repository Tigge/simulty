
#ifndef _BUILDING_FACTORY_HPP_
#define _BUILDING_FACTORY_HPP_

#include "Point.hpp"
#include "Map.hpp"
#include "Building.hpp"
#include "BuildingZone.hpp"
#include "BuildingLoader.hpp"

#include <map>

class BuildingFactory {
    private:

    static bool inited;
    static std::map<int, BuildingInformation *> buildings;

    public:
    
    static void init();
    
    static void loadBuildingInformation(std::string);

    static Building *getBuilding(int buildingType, Point position,
        unsigned char owner, Date built);
    static BuildingZone *getBuildingZone(int type, Point position,
        unsigned char owner, int w, int h, Date built, int level, int style);
    

};

#endif

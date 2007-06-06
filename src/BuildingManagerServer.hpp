
#ifndef _BUILDINGMANAGERSERVER_H_
#define _BUILDINGMANAGERSERVER_H_

#include "Point.hpp"
#include "Map.hpp"
#include "Building.hpp"
#include "BuildingManager.hpp"


class BuildingManagerServer : public BuildingManager {


  public:
    BuildingManagerServer();
    virtual ~BuildingManagerServer();


    virtual void updateZoneBuildings(unsigned char player_slot, Map *map);
    virtual void addZoneBuilding(unsigned char player_slot, int buildingType, Point p, int w, int h);
};

#endif

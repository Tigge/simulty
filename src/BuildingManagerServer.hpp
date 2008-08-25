
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


    virtual void updateZoneBuildings(Player * player, Map *map, Date date);
    virtual void addZoneBuilding(unsigned char player_slot, int buildingType,
        Point p, int w, int h, Date built, int level, int style);
    virtual void removeZoneBuilding(unsigned int id);

    virtual void removeSpecialBuilding(unsigned int id);

    void clearArea(Map *map, Point from, Point to);
};

#endif

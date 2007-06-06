
#ifndef _BUILDINGMANAGER_H_
#define _BUILDINGMANAGER_H_

#include <vector>
#include "Map.hpp"
#include "Building.hpp"


class BuildingManager {

  protected:
    std::vector<Building *> zone_buildings;
    std::vector<Building *> special_buildings;

  public:
    virtual ~BuildingManager();

    // Thrive values:
    int getCrimeThrive(Map *m, char slot, Point where);
    int getConnectionThrive(Map *m, char slot, Point where);
    int getElectricityThrive(Map *m, char slot, Point where);
    int getTaxesThrive(char slot);

    int getThriveValue(Map *m, char slot, Point where);

    // Buildings:
    void addSpecialBuilding(Building *);
    void removeSpecialBuilding(unsigned int id);

    Building *getSpecialBuilding(int i);
    unsigned int getSpecialBuildingCount();
    int getSpecialBuildingID(Point at);

    Building *getZoneBuilding(int i);
    unsigned int getZoneBuildingCount();

    bool canBuildSpecialBuilding(Building *b, unsigned char slot, Map *m);
    bool canBuild(Point at, unsigned char slot, Map *m);

    void updateZoneBuildings(unsigned char player_slot, Map *map);
    virtual void addZoneBuilding(unsigned char player_slot, int buildingType, Point p, int w, int h);
};

#endif

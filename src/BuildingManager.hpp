
#ifndef _BUILDING_MANAGER_H_
#define _BUILDING_MANAGER_H_

#include <vector>
#include "Map.hpp"
#include "Building.hpp"


class BuildingManager {

  private:

    std::vector<Building *> zone_buildings;
    std::vector<Building *> special_buildings;

  public:
    ~BuildingManager();

    // Thrive values:
    int getThriveValueForCrime(Map *m, char slot, Point where);
    int getThriveValueForConnection(Map *m, char slot, Point where);
    int getThriveValueForElectricity(Map *m, char slot, Point where);
    int getThriveValueForTaxes(char slot);

    int getThriveValue(Map *m, char slot, Point where);


    // Buildings:
    void addSpecialBuilding(Building *);
    void removeSpecialBuilding(unsigned int id);

    Building *getSpecialBuilding(int i);
    int getSpecialBuildingCount();
    int getSpecialBuildingID(Point at);

    bool canBuildSpecialBuilding(Building *b, unsigned char slot, Map *m);
    bool canBuild(Point at, unsigned char slot, Map *m);

    void updateZoneBuildings(unsigned char player_slot, Map *map);
    void addZoneBuilding(unsigned char player_slot, int buildingType, Point p, int w, int h);
};

#endif

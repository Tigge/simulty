
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
  
    // Thrive values:
    int getThriveValueForCrime(Map *m, char slot, Point where);  
    int getThriveValueForConnection(Map *m, char slot, Point where);
    int getThriveValueForElectricity(Map *m, char slot, Point where);
    int getThriveValueForTaxes(char slot);
  
    int getThriveValue(Map *m, char slot, Point where);


    // Buildings:
    void addSpecialBuilding(Building *);
    
    Building *getSpecialBuilding(int i);    
    int  getSpecialBuildingCount();
};

#endif

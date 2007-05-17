
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
  
  
    int thrive_value_connection_get(Map *m, char slot, int x, int y);
    int thrive_value_electricity_get(Map *m, char slot, int x, int y);
    int thrive_value_taxes_get(char slot);
  
    int thrive_value_get(Map *m, char slot, int x, int y);


    void addSpecialBuilding(Building *);
};

#endif

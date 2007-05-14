
#ifndef _BUILDING_MANAGER_H_
#define _BUILDING_MANAGER_H_

#include <vector>
#include "map.h"
#include "building.h"


class building_manager {

  private:

    std::vector<building *> zone_buildings;
    std::vector<building *> special_buildings;
  
  public:
  
    int thrive_value_connection_get(map *m, char slot, int x, int y);
    int thrive_value_electricity_get(map *m, char slot, int x, int y);
    int thrive_value_taxes_get(char slot);
  
  
    int thrive_value_get(map *m, char slot, int x, int y);

};

#endif

#include "BuildingManager.hpp"
#include "Client.hpp"


int BuildingManager::getThriveValueForCrime(Map *m, char slot, Point where) {

  // Out of bounds:
  if(where.getX() < 0 || where.getY() < 0 || where.getX() >= m->getWidth() 
      || where.getY() >= m->getHeight()) 
    return 0;

  int distance = 100;

  for(int i = 0; i < getSpecialBuildingCount(); i++) {
    
    if(getSpecialBuilding(i)->getType() == Building::TYPE_POLICE) {
        
      for(int x = 0; x < getSpecialBuilding(i)->getWidth(); x++) 
        for(int y = 0; y < getSpecialBuilding(i)->getHeight(); y++) {
          
          int d = Point::distance(where, 
              getSpecialBuilding(i)->getPosition() + Point(x, y));
                
          if(d < distance ) {
                distance = d;
           
          }                
      }      
    }
  }
  
  

  if(distance < 15)
    return 20;
  else if(distance < 30)
    return 10;
  else  
    return 0;  
}


int BuildingManager::getThriveValueForConnection(Map *map, char slot, Point where) {

    // Out of bounds:
    if(where.getX() < 0 || where.getY() < 0 || where.getX() >= map->getWidth() 
            || where.getY() >= map->getHeight()) 
        return 0;
    

    for(int x = where.getX() - 3; x <= where.getX() + 3; x++)
        for(int y = where.getY() - 3; y <= where.getY() + 3; y++)
            if(map->getTile(x, y)->getOwner() == slot 
                    && map->getTile(x, y)->isRoad())return 20;

    return 0;

}

int BuildingManager::getThriveValueForElectricity(Map *map, char slot, Point where) {

    return 0;

}

int BuildingManager::getThriveValueForTaxes(char slot) {

    return 0;

}

int BuildingManager::getThriveValue(Map *map, char slot, Point where) {

    return getThriveValueForConnection(map, slot, where)
            + getThriveValueForElectricity(map, slot, where)
            + getThriveValueForTaxes(slot) 
            + getThriveValueForCrime(map, slot, where);

}


void BuildingManager::addSpecialBuilding(Building *b) {

    special_buildings.push_back(b);

}

Building *BuildingManager::getSpecialBuilding(int i) {
    return special_buildings[i];
}
int BuildingManager::getSpecialBuildingCount() {
    return special_buildings.size();
}


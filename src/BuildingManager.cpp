#include "BuildingManager.hpp"
#include "Client.hpp"

BuildingManager::~BuildingManager() {

  for(unsigned int i = 0; i < specialBuildings.size(); i++)
    delete specialBuildings[i];

  for(unsigned int i = 0; i < zoneBuildings.size(); i++)
    delete zoneBuildings[i];
}

int BuildingManager::getCrimeThrive(Map *m, char slot, Point where) {

  // Out of bounds:
  if(where.getX() < 0 || where.getY() < 0
      || (unsigned int)where.getX() >= m->getWidth()
      || (unsigned int)where.getY() >= m->getHeight())
    return 0;

  int distance = 100;

  for(unsigned int i = 0; i < getSpecialBuildingCount(); i++) {
    if(getSpecialBuilding(i)->getType() == Building::TYPE_POLICE
        && getSpecialBuilding(i)->getOwner() == slot) {
      for(unsigned int x = 0; x < getSpecialBuilding(i)->getWidth(); x++) {
        for(unsigned int y = 0; y < getSpecialBuilding(i)->getHeight(); y++) {

          int d = Point::distance(where,
              getSpecialBuilding(i)->getPosition() + Point(x, y));

          if(d < distance ) {
                distance = d;

          }
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


int BuildingManager::getConnectionThrive(Map *map, char slot, Point where) {

  // Out of bounds:
  if(map->outOfBounds(where))
    return 0;

  for(int x = where.getX() - 3; x <= where.getX() + 3; x++)
    for(int y = where.getY() - 3; y <= where.getY() + 3; y++)
      if(abs(where.getX() - x) + abs(where.getY() - y) <= 3
          && !map->outOfBounds(Point(x, y))
          && map->getTile(x, y)->getOwner() == slot
          && map->getTile(x, y)->isRoad()) return 20;

  return 0;

}

int BuildingManager::getElectricityThrive(Map *map, char slot, Point where) {

    return 0;

}

int BuildingManager::getTaxesThrive(char slot) {

    return 0;

}

int BuildingManager::getThriveValue(Map *map, char slot, Point where) {

    return getConnectionThrive(map, slot, where)
            + getElectricityThrive(map, slot, where)
            + getTaxesThrive(slot)
            + getCrimeThrive(map, slot, where);

}

int BuildingManager::getThriveLevel(Map *map, char owner, Point where) {

  int con = getConnectionThrive(map, owner, where);
  int el  = getElectricityThrive(map, owner, where);
  int cr  = getCrimeThrive(map, owner, where);
  
  if(con < 20 || cr < 20)
    return 0;
  else {
    if(cr < 10)
      return 1;
    else 
      return 2;  
  }
}

void BuildingManager::addSpecialBuilding(Building *b) {

  specialBuildings.push_back(b);
}

void BuildingManager::removeSpecialBuilding(unsigned int id) {

  specialBuildings.erase(std::vector<Building *>::iterator(specialBuildings.begin() + id));
}

Building *BuildingManager::getSpecialBuilding(int i) {
    return specialBuildings[i];
}
unsigned int BuildingManager::getSpecialBuildingCount() {
    return specialBuildings.size();
}

Building *BuildingManager::getZoneBuilding(int i) {
    return zoneBuildings[i];
}
unsigned int BuildingManager::getZoneBuildingCount() {
    return zoneBuildings.size();
}

bool BuildingManager::canBuild(Point at, unsigned char slot, Map *m) {
  int x = at.getX(), y = at.getY();

  Tile *t = m->getTile(x, y);

  if(t->getOwner() != slot) {
    std::cerr << "The tile " << x << ", " << y << " is not owned by player " << (int)slot << " but " << (int)t->getOwner() << std::endl;
    return false;
  }
  if(t->isRoad()) {
    std::cerr << "The tile " << x << ", " << y << " has a road on it" << std::endl;
    return false;
  }
  if(t->getHouse() != 0) {
    std::cerr << "The tile " << x << ", " << y << " has a building on it" << std::endl;
    return false;
  }
  if(t->getZone() != 0) {
    std::cerr << "The tile " << x << ", " << y << " is zoned" << std::endl;
    return false;
  }


  for(unsigned int i = 0; i < specialBuildings.size(); i++) {
    if(x >= specialBuildings[i]->position.getX() 
        && x < specialBuildings[i]->position.getX()
        + specialBuildings[i]->getWidth()
        && y >= specialBuildings[i]->position.getY() 
        && y < specialBuildings[i]->position.getY() 
        + specialBuildings[i]->getHeight()) {
      //std::cerr << "The tile " << x << ", " << y << " has a special building on it" << std::endl;
      return false;
    }
  }

  return true;
}

bool BuildingManager::canBuildSpecialBuilding(Building *b, unsigned char slot, Map *m) {

  for(int x = b->getPosition().getX(); x < b->getPosition().getX() + b->getWidth(); x++)
    for(int y = b->getPosition().getY(); y < b->getPosition().getY() + b->getHeight(); y++)
      if(!canBuild(Point(x, y), slot, m))
      {
        std::cerr << "Unable to build at " << x << ", " << y << " " << (int)slot << std::endl;
        return false;
      }

  return true;
}

int BuildingManager::getSpecialBuildingID(Point at) {

  for(unsigned int id = 0; id < specialBuildings.size(); id++) {
    if(at.getX() >= specialBuildings[id]->position.getX() 
        && at.getX() < specialBuildings[id]->position.getX() 
        + specialBuildings[id]->getWidth()
        && at.getY() >= specialBuildings[id]->position.getY() 
        && at.getY() < specialBuildings[id]->position.getY() 
        + specialBuildings[id]->getHeight()) {
      return id;
    }
  }

  return -1;
}

void BuildingManager::addZoneBuilding(unsigned char owner, int buildingType,
    Point p, int w, int h, Date built, int level, int style) {

  //std::cout << "Attempting to construct a " << w << "x" << h << " zone of type " << buildingType << "...";
  BuildingZone *b = BuildingFactory::getBuildingZone(buildingType, p, owner,
      w, h, built, level, style);

  zoneBuildings.push_back(b);

  //std::cout << "  Success!" << std::endl;
}

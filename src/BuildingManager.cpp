#include "BuildingManager.hpp"
#include "Client.hpp"

BuildingManager::~BuildingManager() {

  for(unsigned int i = 0; i < special_buildings.size(); i++)
    delete special_buildings[i];

  for(unsigned int i = 0; i < zone_buildings.size(); i++)
    delete zone_buildings[i];
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
      if(!map->outOfBounds(Point(x, y)))
        if(map->getTile(x, y)->getOwner() == slot
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


void BuildingManager::addSpecialBuilding(Building *b) {

  special_buildings.push_back(b);
}

void BuildingManager::removeSpecialBuilding(unsigned int id) {

  special_buildings.erase(std::vector<Building *>::iterator(special_buildings.begin() + id));
}

Building *BuildingManager::getSpecialBuilding(int i) {
    return special_buildings[i];
}
unsigned int BuildingManager::getSpecialBuildingCount() {
    return special_buildings.size();
}

Building *BuildingManager::getZoneBuilding(int i) {
    return zone_buildings[i];
}
unsigned int BuildingManager::getZoneBuildingCount() {
    return zone_buildings.size();
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


  for(unsigned int i = 0; i < special_buildings.size(); i++) {
    if(x >= special_buildings[i]->position.getX() && x < special_buildings[i]->position.getX() + special_buildings[i]->getWidth()
    && y >= special_buildings[i]->position.getY() && y < special_buildings[i]->position.getY() + special_buildings[i]->getHeight()) {
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

  for(unsigned int id = 0; id < special_buildings.size(); id++) {
    if(at.getX() >= special_buildings[id]->position.getX() && at.getX() < special_buildings[id]->position.getX() + special_buildings[id]->getWidth()
    && at.getY() >= special_buildings[id]->position.getY() && at.getY() < special_buildings[id]->position.getY() + special_buildings[id]->getHeight()) {
      return id;
    }
  }

  return -1;
}

void BuildingManager::addZoneBuilding(unsigned char player_slot, int buildingType, Point p, int w, int h) {

  //std::cout << "Attempting to construct a " << w << "x" << h << " zone of type " << buildingType << "...";
  Building *b = BuildingFactory::getBuilding(buildingType, p, player_slot);

  b->setWidth(w);
  b->setHeight(h);

  zone_buildings.push_back(b);

  //std::cout << "  Success!" << std::endl;
}

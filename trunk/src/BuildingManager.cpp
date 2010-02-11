#include "BuildingManager.hpp"
#include "Client.hpp"

BuildingManager::~BuildingManager() {

  for(unsigned int i = 0; i < specialBuildings.size(); i++)
    delete specialBuildings[i];

  for(unsigned int i = 0; i < zoneBuildings.size(); i++)
    delete zoneBuildings[i];
}

void BuildingManager::addSpecialBuilding(Map *m, Building *b) {

  specialBuildings.push_back(b);
  m->markBuilding(b, SIMULTY_BUILDING_SPECIAL);

}

void BuildingManager::removeSpecialBuilding(Map *m, unsigned int id) {
  if((unsigned int)id > zoneBuildings.size()) throw SIMULTYEXCEPTION("\n  removeSpecialBuilding: i > size");

  std::vector<Building *>::iterator b = specialBuildings.begin() + id;
  m->unmarkBuilding((*b));
  specialBuildings.erase(b);

}

Building *BuildingManager::getSpecialBuilding(int i) {
  if((unsigned int)i > zoneBuildings.size()) throw SIMULTYEXCEPTION("\n  getSpecialBuilding: i > size");
    return specialBuildings[i];
}
unsigned int BuildingManager::getSpecialBuildingCount() {
    return specialBuildings.size();
}

BuildingZone *BuildingManager::getZoneBuilding(int i) {
  if((unsigned int)i > zoneBuildings.size()) throw SIMULTYEXCEPTION("\n  getZoneBuilding: i > size");

  return zoneBuildings[i];
}
unsigned int BuildingManager::getZoneBuildingCount() {
    return zoneBuildings.size();
}

bool BuildingManager::canBuild(Point at, unsigned char slot, Map *m) {

  if(m->outOfBounds(at) || m->getTile(at)->getOwner() != slot 
      || m->getTile(at)->isOccupied()) {
    return false;
  }
  return true;
}

bool BuildingManager::canBuildSpecialBuilding(Building *b, unsigned char slot, Map *m) {

  for(int x = b->getPosition().getX(); x < b->getPosition().getX() + b->getWidth(); x++)
    for(int y = b->getPosition().getY(); y < b->getPosition().getY() + b->getHeight(); y++)
      if(!canBuild(Point(x, y), slot, m)) {
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

int BuildingManager::getZoneBuildingID(Point at) {

  for(unsigned int id = 0; id < zoneBuildings.size(); id++) {
    if(at.getX() >= zoneBuildings[id]->position.getX()
        && at.getX() < zoneBuildings[id]->position.getX()
        + zoneBuildings[id]->getWidth()
        && at.getY() >= zoneBuildings[id]->position.getY()
        && at.getY() < zoneBuildings[id]->position.getY()
        + zoneBuildings[id]->getHeight()) {
      return id;
    }
  }

  return -1;
}

void BuildingManager::addZoneBuilding(Map *m, unsigned char owner, 
    int buildingType, Point p, int w, int h, Date built, int level, int style) {

  //std::cout << "Attempting to construct a " << w << "x" << h << " zone of type " << buildingType << "...";
  BuildingZone *b = BuildingFactory::getBuildingZone(buildingType, p, owner,
      w, h, built, level, style);

  zoneBuildings.push_back(b);
  m->markBuilding(b, SIMULTY_BUILDING_ZONE);

  //std::cout << "  Success!" << std::endl;
}

void BuildingManager::removeZoneBuilding(Map *m, unsigned int id) {
  if((unsigned int)id > zoneBuildings.size()) throw SIMULTYEXCEPTION("\n  removeZoneBuilding: i > size");

  std::vector<BuildingZone *>::iterator b = zoneBuildings.begin() + id;
  m->unmarkBuilding((*b));
  zoneBuildings.erase(b);
}

int BuildingManager::bulldozeCost(unsigned char player_slot, Map *m, Point from, Point to) {

  std::cout << "Bulldozing from " << from << " to " << to << std::endl;

  int cost = 0;;

  for(unsigned int i = 0; i < getSpecialBuildingCount(); i++) {
    Building *b = getSpecialBuilding(i);
    Point     p1 = b->getPosition();
    Point     p2 = b->getPosition() + Point(b->getWidth(), b->getHeight());
    
    if(b->getOwner() == player_slot) {
      if(p1.inArea(from, to) || p2.inArea(from, to)) {
        std::cout << "SBuilding in " << p1 << " is in demolition area" << std::endl;
        cost += 100;
      }
    }
  }
  
  for(unsigned int i = 0; i < getZoneBuildingCount(); i++) {
    BuildingZone *b = getZoneBuilding(i);
    Point     p1 = b->getPosition();
    Point     p2 = b->getPosition() + Point(b->getWidth(), b->getHeight());

    if(b->getOwner() == player_slot) {
      if(p1.inArea(from, to) || p2.inArea(from, to)) {
        std::cout << "ZBuilding in " << p1 << " is in demolition area" << std::endl;
        cost += 20 * b->getLevel();
      }
    }
  }
  
  
  return cost;

}

void BuildingManager::bulldoze(unsigned char player_slot, Map *m, Point from, Point to) {

  for(unsigned int i = 0; i < getSpecialBuildingCount();) {
    Building *b = getSpecialBuilding(i);
    Point     p1 = b->getPosition();
    Point     p2 = b->getPosition() + Point(b->getWidth(), b->getHeight());

    if(b->getOwner() == player_slot) {
      if(p1.inArea(from, to) || p2.inArea(from, to)) {
        BuildingManager::removeSpecialBuilding(m, i);
        std::cout << "SBuilding in " << p1 << " is in demoled" << std::endl;
        continue;
      }
    }
    i++;
  }
  
  for(unsigned int i = 0; i < getZoneBuildingCount();) {
    BuildingZone *b = getZoneBuilding(i);
    Point     p1 = b->getPosition();
    Point     p2 = b->getPosition() + Point(b->getWidth(), b->getHeight());

    std::cout << "ZBuilding in " << p1 << " ";

    if(b->getOwner() == player_slot) {
      if(p1.inArea(from, to) || p2.inArea(from, to)) {
        BuildingManager::removeZoneBuilding(m, i);
        continue;
        std::cout << " is in demoled" << std::endl;
      }
    }
    i++;
  }
}

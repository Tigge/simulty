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
  if(m->outOfBounds(where))
    return 0;

  int distance = 0;

  for(unsigned int i = 0; i < getSpecialBuildingCount(); i++) {
    if(getSpecialBuilding(i)->getType() == Building::TYPE_POLICE
        && getSpecialBuilding(i)->getOwner() == slot) {

      for(unsigned int x = 0; x < getSpecialBuilding(i)->getWidth(); x++) {
        for(unsigned int y = 0; y < getSpecialBuilding(i)->getHeight(); y++) {

          int d = Point::distance(where,
              getSpecialBuilding(i)->getPosition() + Point(x, y));

          if(distance == 0 || d < distance ) {
                distance = d;

          }
        }
      }
    }
  }

  // Without ANY police, returns -70. Yes, if the police station is farther, this lessens the value.
  // But then maybe this would be the case in the real world. Say your store is being robbed, when
  // would you complain the most, when there were no police, or when it took the police one hour to
  // get there, and the theifs left 10 minutes ago?
  if(distance == 0) distance = 100;

  // You dont think: "Yay!! A police station, I wanna live there!!", but the area gets a bad
  // reputation if there isn't one. Will only return < 1. (TODO: Good reputation (randomed statistics?))
  if(distance < 30)
    return 0;
  else
    return 30-distance;
}


int BuildingManager::getConnectionThrive(Map *map, char slot, Point where) {

  // Out of bounds:
  if(map->outOfBounds(where))
    return 0;

  if(map->getAdjacentRoads(where))
    return 1;

  return 0;
}

int BuildingManager::getElectricityThrive(Map *map, char slot, Point where) {

    return 0;

}

int BuildingManager::getTaxesThrive(char slot) {

    return 0;
}

int BuildingManager::getCommersialThrive(Map *m, char slot, Point where) {
  return 0;
}
int BuildingManager::getIndustrialThrive(Map *m, char slot, Point where) {
  return 0;
}
int BuildingManager::getResidentialThrive(Map *m, char slot, Point where) {
  if(m->outOfBounds(where))
    return 0;

  // Not checking to ensure that buildings are connected to roads. They should
  // not be built on if they arent, unless there is two "sub-nets".
  unsigned int jobs = 0, store_dist = 0;

  // Check for available jobs, and distance to closes store.
  for(unsigned int i = 0; i < zoneBuildings.size(); i++) {
    BuildingZone *b = zoneBuildings[i];
    if(b->getType() != Building::TYPE_RESIDENTIAL) {  // then you can get a job here
       // Checks at placement should prevent this from going out of bounds
      if(Point::distance(where,
          Point(b->getPosition().getX() + b->getWidth()/2, b->getPosition().getY() + b->getHeight()/2)) < 30) {
        // Employer nearby!
        // TODO: Move job functions to BuildingZone::

        if(b->getType() == Building::TYPE_INDUSTRIAL)
          jobs += b->getWidth()*b->getHeight()*10 - (unsigned int)pow(b->getLevel(), 2.0) + 5;
        else  // Commersial employer
          jobs += b->getWidth()*b->getHeight()*5 - (unsigned int)pow(b->getLevel(), 2.0) + 2;

        for(unsigned int i = 0; i < zoneBuildings.size(); i++) {
          BuildingZone *b = zoneBuildings[i];
          if(b->getType() == Building::TYPE_RESIDENTIAL) {  // then you can get a job here
            if(Point::distance(where,
                Point(b->getPosition().getX() + b->getWidth(), b->getPosition().getY() + b->getHeight())) < 30) {

              // TODO:
              // What if these guys can reach jobs, which u cant reach from $where?
              // Should we spread out jobs as good as they possibly can be?
              // Maybe we need an citizen-class, if we want things to be really neat?
              // Should all people work?
              if(jobs < 0)
              jobs -= b->getInhabitants();
            }
          }
        }
      }
      // Check for closes store
      if(b->getType() == Building::TYPE_COMMERSIAL) {
        unsigned int d = Point::distance(where,
          Point(b->getPosition().getX() + b->getWidth()/2, b->getPosition().getY() + b->getHeight()/2));
        if(store_dist == 0 || d < store_dist) {
          store_dist = d;
        }
      }
    }
  }

  // Calculate thrive
  int thrive = 0;

  if(store_dist == 0)
    thrive -= 2;
  else
    thrive += 2/store_dist;
  if(jobs == 0)
    thrive -= 3;
  else
    thrive += 3*jobs/10;

  return thrive;
}

int BuildingManager::getEnvoirmentThrive(Map *m, char slot, Point where) {

  // Out of bounds:
  if(m->outOfBounds(where))
    return 0;

  int thrive = 0;

  // Function to calculate industrial noise. Other players' industry also counts ;):
  //int noise = 0;
  for(unsigned int i = 0; i < getZoneBuildingCount(); i++) {
    BuildingZone *b = getZoneBuilding(i);
    if(b->getType() == Building::TYPE_INDUSTRIAL) {
      //if(
    }
  }

  return thrive;
}

int BuildingManager::getThrive(Map *map, char slot, Point where) {

  if(map->outOfBounds(where)) return 0;

  if(map->getTile(where)->getZone() == SIMULTY_ZONE_COM) {
    return getConnectionThrive(map, slot, where)
            + getElectricityThrive(map, slot, where)
            + getTaxesThrive(slot)
            + getCrimeThrive(map, slot, where)
            + getEnvoirmentThrive(map, slot,where)
            + getCommersialThrive(map, slot, where);
  }

  else if(map->getTile(where)->getZone() == SIMULTY_ZONE_IND) {
    return getConnectionThrive(map, slot, where)
            + getElectricityThrive(map, slot, where)
            + getTaxesThrive(slot)
            + getCrimeThrive(map, slot, where)
            + getEnvoirmentThrive(map, slot,where)
            + getIndustrialThrive(map, slot, where);
  }

  else if(map->getTile(where)->getZone() == SIMULTY_ZONE_RES) {
    return getConnectionThrive(map, slot, where)
            + getElectricityThrive(map, slot, where)
            + getTaxesThrive(slot)
            + getCrimeThrive(map, slot, where)
            + getEnvoirmentThrive(map, slot,where)
            + getResidentialThrive(map, slot, where);
  }

  return getConnectionThrive(map, slot, where)
          + getElectricityThrive(map, slot, where)
          + getTaxesThrive(slot)
          + getCrimeThrive(map, slot, where)
          + getEnvoirmentThrive(map, slot,where);

}

unsigned int BuildingManager::getThriveLevel(Map *map, char owner, Point where) {

  int thrive = getThrive(map, owner, where);

  if(thrive < 1)
    return 0;

  return thrive/30;
/*
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
  }*/
}
unsigned int BuildingManager::getThriveLevel(int thrive) {

  if(thrive < 1)
    return 0;

  return thrive/30;
}

void BuildingManager::addSpecialBuilding(Building *b) {

  specialBuildings.push_back(b);
}

void BuildingManager::removeSpecialBuilding(unsigned int id) {
  if((unsigned int)id > zoneBuildings.size()) throw SIMULTYEXCEPTION("\n  removeSpecialBuilding: i > size");

  specialBuildings.erase(std::vector<Building *>::iterator(specialBuildings.begin() + id));
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

  if(m->outOfBounds(at)) return false;

  int x = at.getX(), y = at.getY();

  Tile *t = m->getTile(x, y);

  if(t->getOwner() != slot) {
    std::cerr << "The tile " << x << ", " << y << " is not owned by player " << (int)slot << ", but " << (int)t->getOwner() << std::endl;
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

void BuildingManager::addZoneBuilding(unsigned char owner, int buildingType,
    Point p, int w, int h, Date built, int level, int style) {

  //std::cout << "Attempting to construct a " << w << "x" << h << " zone of type " << buildingType << "...";
  BuildingZone *b = BuildingFactory::getBuildingZone(buildingType, p, owner,
      w, h, built, level, style);

  zoneBuildings.push_back(b);

  //std::cout << "  Success!" << std::endl;
}

void BuildingManager::removeZoneBuilding(unsigned int id) {
  if((unsigned int)id > zoneBuildings.size()) throw SIMULTYEXCEPTION("\n  removeZoneBuilding: i > size");

  zoneBuildings.erase(std::vector<BuildingZone *>::iterator(zoneBuildings.begin() + id));
}

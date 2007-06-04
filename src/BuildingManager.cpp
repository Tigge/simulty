#include "BuildingManager.hpp"
#include "Client.hpp"


BuildingManager::~BuildingManager() {

  for(unsigned int i = 0; i < special_buildings.size(); i++)
    delete special_buildings[i];

  for(unsigned int i = 0; i < zone_buildings.size(); i++)
    delete zone_buildings[i];
}

int BuildingManager::getThriveValueForCrime(Map *m, char slot, Point where) {

  // Out of bounds:
  if(where.getX() < 0 || where.getY() < 0 || where.getX() >= m->getWidth()
      || where.getY() >= m->getHeight())
    return 0;

  int distance = 100;

  for(int i = 0; i < getSpecialBuildingCount(); i++) {
    if(getSpecialBuilding(i)->getType() == Building::TYPE_POLICE
        && getSpecialBuilding(i)->getOwner() == slot) {
      for(int x = 0; x < getSpecialBuilding(i)->getWidth(); x++) {
        for(int y = 0; y < getSpecialBuilding(i)->getHeight(); y++) {

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

bool BuildingManager::canBuild(Point at, unsigned char slot, Map *m) {
  int x = at.getX(), y = at.getY();

  Tile *t = m->getTile(x, y);

  if(t->getOwner() != slot) {
    //std::cerr << "The tile " << x << ", " << y << " is not owned by player " << slot << std::endl;
    return false;
  }
  if(t->isRoad()) {
    //std::cerr << "The tile " << x << ", " << y << " has a road on it" << std::endl;
    return false;
  }
  if(t->getHouse() != 0) {
    //std::cerr << "The tile " << x << ", " << y << " has a building on it" << std::endl;
    return false;
  }
  if(t->getZone() != 0) {
    //std::cerr << "The tile " << x << ", " << y << " is zoned" << std::endl;
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
        //std::cerr << "Unable to build at " << x << ", " << y << std::endl;
        return false;
      }

  return true;
}
void BuildingManager::addZoneBuilding(unsigned char player_slot, int buildingType, Point p, int w, int h) {

  Building *b = BuildingFactory::getBuilding(buildingType, p, player_slot);

  b->setWidth(w);
  b->setHeight(h);

  zone_buildings.push_back(b);
  
  std::cout << "Build zone building" << std::endl;
}
void BuildingManager::updateZoneBuildings(unsigned char player_slot, Map *map)
{
  /*//Tear down all old buildings
  for(unsigned int i = 0; i < zone_buildings.size(); i++)
    delete zone_buildings[i];
  zone_buildings = std::vector<Building *>();*/

  int thrive_min_val = 1;
  int max_size = 3;
  std::vector<Point> buildable_tiles;

  // loop through all tiles
  for(int x = 0; x < map->getWidth(); x++) {
    for(int y = 0; y < map->getHeight(); y++) {
      // If the tile is zoned and hasn't got a road on it
      if(map->getTile(x, y)->getZone() != 0 && !map->getTile(x, y)->isRoad()) {

        // Check for special buildings on the zone
        bool special_building = false;
        for(unsigned int i = 0; i < special_buildings.size(); i++) {
          if(x < special_buildings[i]->getPosition().getX() + special_buildings[i]->getWidth()
          && x >= special_buildings[i]->getPosition().getX()
          && y < special_buildings[i]->getPosition().getY() + special_buildings[i]->getHeight()
          && y >= special_buildings[i]->getPosition().getY()) { // then the tile is underneath an already existant building.
            special_building = true; break;
          }
        }
        if(!special_building) {
          // Check for zone buildings on the zone
          bool zone_building = false;
          for(unsigned int i = 0; i < zone_buildings.size(); i++) {
            if(x < zone_buildings[i]->getPosition().getX() + zone_buildings[i]->getWidth()
            && x >= zone_buildings[i]->getPosition().getX()
            && y < zone_buildings[i]->getPosition().getY() + zone_buildings[i]->getHeight()
            && y >= zone_buildings[i]->getPosition().getY()) { // then the tile is underneath an already existant building.
              zone_building = true; break;
            }
          }
          if(zone_building) {
            // If there is one, level up?
          }
          // If there is no building, we can build here. Is this area attractive enough?
          else if(getThriveValue(map, player_slot, Point(x,y)) >= thrive_min_val) {

            // Theoreticly, all zones which meats the minimum value will be inhabited
            int tries = 0;
            while(tries < max_size) {
              tries++;
              bool good = true;
              unsigned char zone = map->getTile(x, y)->getZone();
              int total_thrive = 0,
                  w = 1+rand()%max_size,
                  h = 1+rand()%max_size;

              // Check if the surrounding tiles can join our building project
              for(int tile_x = x; tile_x < x+w; tile_x++) {
                for(int tile_y = y; tile_y < y+h; tile_y++) {
                  total_thrive += getThriveValue(map, player_slot, Point(tile_x, tile_y));
                  if(map->getTile(tile_x, tile_y)->getZone() != zone
                  || map->getTile(tile_x, tile_y)->getOwner() != player_slot
                  || map->getTile(tile_x, tile_y)->isRoad()) {
                    good = false; break;
                  }
                  for(unsigned int i = 0; i < special_buildings.size(); i++) {
                    if(tile_x >= special_buildings[i]->position.getX() && tile_x < special_buildings[i]->position.getX() + special_buildings[i]->getWidth()
                    && tile_y >= special_buildings[i]->position.getY() && tile_y < special_buildings[i]->position.getY() + special_buildings[i]->getHeight()) {
                      good = false; break;
                    }
                  }
                  for(unsigned int i = 0; i < zone_buildings.size(); i++) {
                    if(tile_x >= zone_buildings[i]->position.getX() && tile_x < zone_buildings[i]->position.getX() + zone_buildings[i]->getWidth()
                    && tile_y >= zone_buildings[i]->position.getY() && tile_y < zone_buildings[i]->position.getY() + zone_buildings[i]->getHeight()) {
                      good = false; break;
                    }
                  }
                }
                if(!good)
                  break;
              }
              if(good) {
                int average_thrive = total_thrive/(w*h);
                addZoneBuilding(player_slot, Building::TYPE_RESIDENTIAL, Point(x, y), w, h);
              }
            }
          }
        }
      }
    }
  }
}

#include "Server.hpp"
#include "NL.hpp"
#include "shared.h"

#include "BuildingManagerServer.hpp"

#include "SimultyException.hpp"

BuildingManagerServer::BuildingManagerServer() : BuildingManager() {

}
BuildingManagerServer::~BuildingManagerServer() {

}

void BuildingManagerServer::updateZoneBuildings(unsigned char player_slot, Map *map, Date date) {

  int max_size = 3;

  // loop through all tiles
  for(int x = 0; (unsigned int)x < map->getWidth(); x++) {
    for(int y = 0; (unsigned int)y < map->getHeight(); y++) {
      // If the tile is zoned and hasn't got a road on it
      if(map->getTile(x, y)->getZone() != 0 && !map->getTile(x, y)->isRoad()) {

        // Check for special buildings on the zone
        bool special_building = false;
        for(unsigned int i = 0; i < getSpecialBuildingCount(); i++) {
          if(x < getSpecialBuilding(i)->getPosition().getX() + getSpecialBuilding(i)->getWidth()
          && x >= getSpecialBuilding(i)->getPosition().getX()
          && y < getSpecialBuilding(i)->getPosition().getY() + getSpecialBuilding(i)->getHeight()
          && y >= getSpecialBuilding(i)->getPosition().getY()) { // then the tile is underneath an already existant building.
            special_building = true; break;
          }
        }
        if(!special_building) {
          // Check for zone buildings on the zone
          bool zone_building = false;
          for(unsigned int i = 0; i < getZoneBuildingCount(); i++) {
            if(x < getZoneBuilding(i)->getPosition().getX() + getZoneBuilding(i)->getWidth()
            && x >= getZoneBuilding(i)->getPosition().getX()
            && y < getZoneBuilding(i)->getPosition().getY() + getZoneBuilding(i)->getHeight()
            && y >= getZoneBuilding(i)->getPosition().getY()) { // then the tile is underneath an already existant building.
              zone_building = true; break;
            }
          }
          if(zone_building) {
            // If there is one, level up?

            // sugestion:
            if(rand() % int(420/int(date.monthsPassed()
            - getZoneBuilding(getZoneBuildingID(Point(x,y)))->getBuildDate().monthsPassed())) == 0)
              removeZoneBuilding(getZoneBuildingID(Point(x,y)));
            // next update, there might be a new building here, with more accurate level.
          }
          // If there is no building, we can build here. Is this area attractive enough?
          else if(getThriveLevel(map, player_slot, Point(x,y)) > 0) {

            // Theoreticly, all zones which meats the minimum value will be inhabited
            int tries = 0;
            while(tries < max_size-1) {
              tries++;
              bool good = true;
              unsigned char zone = map->getTile(x, y)->getZone();
              int total_thrive = 0,
                  w = 1+rand()%max_size,
                  h = 1+rand()%max_size;

              if(map->outOfBounds(Point(x+w-1, y+h-1))) {
                good = false;
              }
              else {
                // Check if the surrounding tiles can join our building project,
                // and check whether the building is connected to a road
                bool haveRoad = false;
                for(int tile_x = x; tile_x < x+w; tile_x++) {
                  for(int tile_y = y; tile_y < y+h; tile_y++) {

                    if(map->getAdjacentRoads(Point(tile_x, tile_y)) != 0) {
                      haveRoad = true;
                    }

                    total_thrive += getThriveValue(map, player_slot, Point(tile_x, tile_y));
                    if(map->getTile(tile_x, tile_y)->getZone() != zone
                    || map->getTile(tile_x, tile_y)->getOwner() != player_slot
                    || map->getTile(tile_x, tile_y)->isRoad()) {
                      good = false; break;
                    }
                    for(unsigned int i = 0; i < getSpecialBuildingCount(); i++) {
                      if(tile_x >= getSpecialBuilding(i)->position.getX() && tile_x < getSpecialBuilding(i)->position.getX() + getSpecialBuilding(i)->getWidth()
                      && tile_y >= getSpecialBuilding(i)->position.getY() && tile_y < getSpecialBuilding(i)->position.getY() + getSpecialBuilding(i)->getHeight()) {
                        good = false; break;
                      }
                    }
                    for(unsigned int i = 0; i < getZoneBuildingCount(); i++) {
                      if(tile_x >= getZoneBuilding(i)->position.getX() && tile_x < getZoneBuilding(i)->position.getX() + getZoneBuilding(i)->getWidth()
                      && tile_y >= getZoneBuilding(i)->position.getY() && tile_y < getZoneBuilding(i)->position.getY() + getZoneBuilding(i)->getHeight()) {
                        good = false; break;
                      }
                    }
                  }
                  if(!good)
                    break;
                } if(!haveRoad) good = false;

                if(good) {
                  Server *server = Server::getInstance();
                  int level      = total_thrive/(w*h);
                  if(zone == SIMULTY_ZONE_RES)
                    addZoneBuilding(player_slot, Building::TYPE_RESIDENTIAL,
                        Point(x, y), w, h, server->getDate(), level, 0);
                  else if(zone == SIMULTY_ZONE_IND)
                    addZoneBuilding(player_slot, Building::TYPE_INDUSTRIAL,
                        Point(x, y), w, h, server->getDate(), level, 0);
                  else if(zone == SIMULTY_ZONE_COM)
                    addZoneBuilding(player_slot, Building::TYPE_COMMERSIAL,
                        Point(x, y), w, h, server->getDate(), level, 0);
                  else throw SIMULTYEXCEPTION("Can't build a zonebuilding of a type which shouldn't exist!");
                }
              }
            }
          }
        }
      }
    }
  }
}
void BuildingManagerServer::addZoneBuilding(unsigned char playerSlot,
    int buildingType, Point p, int w, int h, Date built, int level, int style) {

  BuildingManager::addZoneBuilding(playerSlot, buildingType, p, w, h,
      built, level, style);

  Server *server = Server::getInstance();

  NLPacket packet(NLPACKET_TYPE_SIMULTY_BUILD_ZONE_BUILDING);
  packet << (NLINT16)playerSlot << (NLINT16)buildingType << (NLINT32)p.getX()
         << (NLINT32)p.getY()   << (NLINT16)w            << (NLINT16)h
         << (NLINT16)level      << (NLINT16)style;

  server->packet_send_to_all(packet);

}

Building *BuildingManagerServer::getZoneBuilding(int i) {

  return zoneBuildings[i];
}
unsigned int BuildingManagerServer::getZoneBuildingCount() {

  return zoneBuildings.size();
}

void BuildingManagerServer::removeZoneBuilding(unsigned int id) {
  Building *b = getZoneBuilding(id);

  Server *server = Server::getInstance();

  NLPacket packet(NLPACKET_TYPE_SIMULTY_REMOVE_ZONE_BUILDING);
  packet << (NLINT32)b->getPosition().getX() << (NLINT32)b->getPosition().getY();

  server->packet_send_to_all(packet);

  zoneBuildings.erase(std::vector<BuildingZone *>::iterator(zoneBuildings.begin() + id));
}

void BuildingManagerServer::removeSpecialBuilding(unsigned int id) {
  Building *b = getSpecialBuilding(id);

  Server *server = Server::getInstance();

  NLPacket packet(NLPACKET_TYPE_SIMULTY_REMOVE_SPECIAL_BUILDING);
  packet << (NLINT32)b->getPosition().getX() << (NLINT32)b->getPosition().getY();

  server->packet_send_to_all(packet);

  specialBuildings.erase(std::vector<Building *>::iterator(specialBuildings.begin() + id));
}

void BuildingManagerServer::clearArea(Map *map, Point from, Point to) {

  for(unsigned int x = from.getX(); x <= (unsigned int)to.getX() && x < map->getWidth(); x++) {
    for(unsigned int y = from.getX(); y <= (unsigned int)to.getX() && y < map->getHeight(); y++) {

      if(getSpecialBuildingID(Point(x, y)) != -1)
        removeSpecialBuilding(getSpecialBuildingID(Point(x, y)));

      if(getZoneBuildingID(Point(x, y)) != -1)
        removeZoneBuilding(getZoneBuildingID(Point(x, y)));

    }
  }
}

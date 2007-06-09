#include "Server.hpp"
#include "NL.hpp"
#include "shared.h"

#include "BuildingManagerServer.hpp"

#include "SimultyException.hpp"

BuildingManagerServer::BuildingManagerServer() : BuildingManager() {

}
BuildingManagerServer::~BuildingManagerServer() {

}

void BuildingManagerServer::updateZoneBuildings(unsigned char player_slot, Map *map) {

  int thrive_min_val = 1;
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
            //[if(rand() % X == 42) DeleteBuilding]
            // next update, it will be rebuilt, with higher standards.
          }
          // If there is no building, we can build here. Is this area attractive enough?
          else if(getThriveValue(map, player_slot, Point(x,y)) >= thrive_min_val) {

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
                // Check if the surrounding tiles can join our building project
                for(int tile_x = x; tile_x < x+w; tile_x++) {
                  for(int tile_y = y; tile_y < y+h; tile_y++) {

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
                }
                if(good) {
                  //int average_thrive = total_thrive/(w*h);
                  if(zone == SIMULTY_ZONE_RES)
                    addZoneBuilding(player_slot, Building::TYPE_RESIDENTIAL, Point(x, y), w, h);
                  else if(zone == SIMULTY_ZONE_IND)
                    addZoneBuilding(player_slot, Building::TYPE_INDUSTRIAL, Point(x, y), w, h);
                  else if(zone == SIMULTY_ZONE_COM)
                    addZoneBuilding(player_slot, Building::TYPE_COMMERSIAL, Point(x, y), w, h);
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
void BuildingManagerServer::addZoneBuilding(unsigned char playerSlot, int buildingType, Point p, int w, int h) {

  BuildingManager::addZoneBuilding(playerSlot, buildingType, p, w, h);

  Server *server = Server::getInstance();

  NLPacket packet(NLPACKET_TYPE_SIMULTY_ZONE_BUILDING);
  packet << (NLINT16)playerSlot << (NLINT16)buildingType << (NLINT32)p.getX()
         << (NLINT32)p.getY()   << (NLINT16)w            << (NLINT16)h;

  server->packet_send_to_all(packet);

}

Building *BuildingManagerServer::getZoneBuilding(int i) {
    return zone_buildings[i];
}
unsigned int BuildingManagerServer::getZoneBuildingCount() {
    return zone_buildings.size();
}

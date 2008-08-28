#include "Server.hpp"
#include "NL.hpp"
#include "shared.h"

#include "BuildingManagerServer.hpp"

#include "SimultyException.hpp"

BuildingManagerServer::BuildingManagerServer() : BuildingManager() {

}
BuildingManagerServer::~BuildingManagerServer() {

}

void BuildingManagerServer::updateZoneBuildings(Player *player, Map *map, Date date) {
  
  int        max_size = 3;
  ThriveMap *tm       = player->getThriveMap();

  // loop through all tiles
  for(int x = 0; (unsigned int)x < map->getWidth(); x++) {
    for(int y = 0; (unsigned int)y < map->getHeight(); y++) {
      // If the tile is zoned and hasn't got a road on it
      if(map->getTile(x, y)->getZone() != 0 && !map->getTile(x, y)->isRoad()) {

        // Check for special buildings on the zone (TODO, should not be able to
        // happend. When placing special buildings zone should be sold)
        bool special_building = false;
        for(unsigned int i = 0; i < getSpecialBuildingCount(); i++) {
          if(x < getSpecialBuilding(i)->getPosition().getX() + getSpecialBuilding(i)->getWidth()
          && x >= getSpecialBuilding(i)->getPosition().getX()
          && y < getSpecialBuilding(i)->getPosition().getY() + getSpecialBuilding(i)->getHeight()
          && y >= getSpecialBuilding(i)->getPosition().getY()) { // then the tile is underneath a specialbuilding.
            special_building = true; break;
          }
        }
        
        // No special buildings on zone spot
        if(!special_building) {
        
          // Check for zone buildings on the zone
          int zone_building_id = getZoneBuildingID(Point(x,y));

          if(zone_building_id != -1) {  // then there is a zone building here!

            BuildingZone *zb = getZoneBuilding(zone_building_id);

            // Get thrive level for zone building:
            double zb_tot_thrive = 0;
            for(int zb_x = zb->getPosition().getX(); zb_x < zb->getPosition().getX() + zb->getWidth(); zb_x++)
              for(int zb_y = zb->getPosition().getY(); zb_y < zb->getPosition().getY() + zb->getHeight(); zb_y++)
                zb_tot_thrive += tm->getThrive(Point(x, y));
            zb_tot_thrive /= zb->getWidth() * zb->getHeight();

            
            // Raise building because it is doesn't fit (to fancy i.e. to 
            // expensive to live in in this area or to shabby which means no 
            // one wants to live there either)
            
            // TODO: how to convert thrive value into appropriate building
            //       levels? Use (unsigned int)(zb_tot_thrive * 10) for now.
            
            unsigned int level_zb = (unsigned int)(zb_tot_thrive * 10);
            
            if(level_zb != zb->getLevel()) {
              // Is the building unsuitable in this area?

              // TODO: significantly increase wanna-move if ohters are already moving.

              int level_diff = abs(level_zb);
              if(level_diff > 0) {
                // TODO: Better chance-formula
                if(rand() % 10/level_diff == int(42/int(Date::daysToMonths(date.getTimeDiff(zb->getBuildDate())) * level_diff)) == 0) {
                  // Depending on how old it is and how greatly it missfits, it might be razed
                  removeZoneBuilding(map, getZoneBuildingID(Point(x,y)));
                }
              }
            }
          }
          // If there is no building, we can build here. Is this area attractive enough?
          else if(tm->getThrive(Point(x, y)) > 0) {
          //else if(getThriveLevel(map, player->getSlot(), Point(x,y)) > 0) {
            
            std::cout << "Thr > 0 at " << x << ", " << y << " = " << tm->getThrive(Point(x, y)) << std::endl;
            
            // Theoreticly, all zones which meats the minimum value will be inhabited
            int tries = 0;
            while(tries < max_size) {
              tries++;
              bool good = true;
              unsigned char zone = map->getTile(x, y)->getZone();
              double total_thrive = 0,
                  w = 1+rand()%max_size,
                  h = 1+rand()%max_size;

              std::cout << " - Width: " << w << ", Height: " << h << std::endl;

              if(map->outOfBounds(Point(x+w-1, y+h-1))) {
                good = false;
              } else {
                // Check if the surrounding tiles can join our building project,
                // and check whether the building is connected to a road
                bool haveRoad = false;
                for(int tile_x = x; tile_x < x+w; tile_x++) {
                  for(int tile_y = y; tile_y < y+h; tile_y++) {

                    if(map->getAdjacentRoads(Point(tile_x, tile_y)) != 0) {
                      haveRoad = true;
                      std::cout << " - have road" << std::endl;
                    }
                    
                    total_thrive += tm->getThrive(Point(tile_x, tile_y));
                    if(map->getTile(tile_x, tile_y)->getZone() != zone
                    || map->getTile(tile_x, tile_y)->getOwner() != player->getSlot()
                    || map->getTile(tile_x, tile_y)->isRoad()) {
                      good = false; break;
                      std::cout << "  ! No zone, not owned or road" << std::endl;
                    }
                    for(unsigned int i = 0; i < getSpecialBuildingCount(); i++) {
                      if(tile_x >= getSpecialBuilding(i)->position.getX() && tile_x < getSpecialBuilding(i)->position.getX() + getSpecialBuilding(i)->getWidth()
                      && tile_y >= getSpecialBuilding(i)->position.getY() && tile_y < getSpecialBuilding(i)->position.getY() + getSpecialBuilding(i)->getHeight()) {
                        good = false; break;
                      std::cout << "  ! Occupied by special building" << std::endl;
                      }
                    }
                    for(unsigned int i = 0; i < getZoneBuildingCount(); i++) {
                      if(tile_x >= getZoneBuilding(i)->position.getX() && tile_x < getZoneBuilding(i)->position.getX() + getZoneBuilding(i)->getWidth()
                      && tile_y >= getZoneBuilding(i)->position.getY() && tile_y < getZoneBuilding(i)->position.getY() + getZoneBuilding(i)->getHeight()) {
                        good = false; break;
                        std::cout << "  ! Occupied by zone building" << std::endl;
                      }
                    }
                  }
                  if(!good)
                    break;
                } if(!haveRoad) {
                  good = false;
                  std::cout << "  ! No road" << std::endl;
                }

                if(good) {
                  std::cout << " - All good :) " << std::endl;
                  Server *server = Server::getInstance();
                  int level      = (unsigned int)(total_thrive / (w*h) * 100);
                  
                  std::cout << " Total thrive = " << total_thrive << std::endl;
                  std::cout << " LEVELD = " << (total_thrive / (w*h) * 100) << std::endl;
                  std::cout << " LEVEL  = " << level << std::endl;
                  
                  if(level > 0) {

                    if(zone == SIMULTY_ZONE_RES)
                      addZoneBuilding(map, player->getSlot(), Building::TYPE_RESIDENTIAL,
                          Point(x, y), w, h, server->getDate(), level, 0);
                    else if(zone == SIMULTY_ZONE_IND)
                      addZoneBuilding(map, player->getSlot(), Building::TYPE_INDUSTRIAL,
                          Point(x, y), w, h, server->getDate(), level, 0);
                    else if(zone == SIMULTY_ZONE_COM)
                      addZoneBuilding(map, player->getSlot(), Building::TYPE_COMMERSIAL,
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
}

void BuildingManagerServer::addZoneBuilding(Map *m, unsigned char playerSlot,
    int buildingType, Point p, int w, int h, Date built, int level, int style) {

  BuildingManager::addZoneBuilding(m, playerSlot, buildingType, p, w, h,
      built, level, style);

  Server *server = Server::getInstance();

  NLPacket packet(NLPACKET_TYPE_SIMULTY_BUILD_ZONE_BUILDING);
  packet << (NLINT16)playerSlot << (NLINT16)buildingType << (NLINT32)p.getX()
         << (NLINT32)p.getY()   << (NLINT16)w            << (NLINT16)h
         << (NLINT16)level      << (NLINT16)style;

  server->packet_send_to_all(packet);

}

void BuildingManagerServer::removeZoneBuilding(Map *map, unsigned int id) {
  Building *b = getZoneBuilding(id);

  Server *server = Server::getInstance();

  NLPacket packet(NLPACKET_TYPE_SIMULTY_REMOVE_ZONE_BUILDING);
  packet << (NLINT32)b->getPosition().getX() << (NLINT32)b->getPosition().getY();

  server->packet_send_to_all(packet);

  BuildingManager::removeZoneBuilding(map, id);
  //zoneBuildings.erase(std::vector<BuildingZone *>::iterator(zoneBuildings.begin() + id));
}

void BuildingManagerServer::removeSpecialBuilding(Map *map, unsigned int id) {
  Building *b = getSpecialBuilding(id);

  Server *server = Server::getInstance();

  NLPacket packet(NLPACKET_TYPE_SIMULTY_REMOVE_SPECIAL_BUILDING);
  packet << (NLINT32)b->getPosition().getX() << (NLINT32)b->getPosition().getY();

  server->packet_send_to_all(packet);

  BuildingManager::removeSpecialBuilding(map, id);
  //specialBuildings.erase(std::vector<Building *>::iterator(specialBuildings.begin() + id));
}

void BuildingManagerServer::clearArea(Map *map, Point from, Point to) {

  for(unsigned int x = from.getX(); x <= (unsigned int)to.getX() && x < map->getWidth(); x++) {
    for(unsigned int y = from.getX(); y <= (unsigned int)to.getX() && y < map->getHeight(); y++) {

      if(getSpecialBuildingID(Point(x, y)) != -1)
        removeSpecialBuilding(map, getSpecialBuildingID(Point(x, y)));

      if(getZoneBuildingID(Point(x, y)) != -1)
        removeZoneBuilding(map, getZoneBuildingID(Point(x, y)));

    }
  }
}

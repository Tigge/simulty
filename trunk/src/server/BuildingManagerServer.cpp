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
      Tile *tile = map->getTile(x, y);
      
      // If the tile is zoned and hasn't got a road on it:
      if(tile->getZone() != SIMULTY_ZONE_NONE && !tile->isRoad()) {
      
        // We have a zone building:
        if(tile->getBuilding() == SIMULTY_BUILDING_ZONE) {
          int           zbi = getZoneBuildingID(Point(x, y));
          BuildingZone *zb  = getZoneBuilding(zbi);
          
          // Get average thrive for zone building: (TODO: Better?)
          double zbThrive = 0;
          for(int zb_x = zb->getPosition().getX(); 
              zb_x < zb->getPosition().getX() + zb->getWidth(); zb_x++) {
            for(int zb_y = zb->getPosition().getY(); 
                zb_y < zb->getPosition().getY() + zb->getHeight(); zb_y++) {
              zbThrive += tm->getThrive(Point(zb_x, zb_y));
            }
          }
          zbThrive /= zb->getWidth() * zb->getHeight();
          
          // Raise building because it is doesn't fit (to fancy i.e. to 
          // expensive to live in in this area or to shabby which means no 
          // one wants to live there either)
          
          // TODO: how to convert thrive value into appropriate building
          //       levels? Use (unsigned int)(zb_tot_thrive * 10) for now.
          
          unsigned int newLevel = (unsigned int)(zbThrive * 10);
           // Is the building unsuitable in this area?
          if(newLevel != zb->getLevel()) {
            // TODO: significantly increase wanna-move if ohters are already moving.
            int levelDiff = abs(newLevel - zb->getLevel());
            if(levelDiff > 0) {
              // TODO: Better chance-formula
              
              int daysOld  = date.getTimeDiff(zb->getBuildDate());
              int monthOld = Date::daysToMonths(daysOld);
              
              // Depending on how old it is and how greatly it missfits, it 
              // might be razed (3 - levelDiff) * 12 (months) + 0 to 24 month
              // should be less then the buildings age.
              // TODO: buildings of greater level should be harder to "kill"
              
              int monthDie = (3 - levelDiff) * 12 + rand() % 24;
              
              if(monthDie < monthOld) {
                removeZoneBuilding(map, getZoneBuildingID(Point(x,y)));
              }
              
              std::cout << "Zone building at " << x << ", " << y << ":" << std::endl;
              std::cout << "ZBBuilt      " << zb->getBuildDate() << std::endl;
              std::cout << "ZBId:        " << zbi << std::endl;
              std::cout << "ZBLevel:     " << zb->getLevel() << std::endl;
              std::cout << "ZBNewLevel:  " << newLevel << std::endl;
              std::cout << "ZBThrive:    " << zbThrive << std::endl;
              std::cout << "ZBMOld       " << monthOld << std::endl;
              std::cout << "ZBMKill      " << monthDie << std::endl;
            }
          } else {
            std::cout << "Zone building at " << x << ", " << y << ":" << std::endl;
            std::cout << "ZBBuilt      " << zb->getBuildDate() << std::endl;
            std::cout << "ZBId:        " << zbi << std::endl;
            std::cout << "ZBLevel:     " << zb->getLevel() << std::endl;
            std::cout << "ZBNewLevel:  " << newLevel << std::endl;
            std::cout << "ZBThrive:    " << zbThrive << std::endl;
          }
         
        // If there is no building, we can build here. Is this area attractive enough?
        } else if(tm->getThrive(Point(x, y)) > 0) {
          
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

                  if(map->isCloseToRoad(Point(tile_x, tile_y))) {
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
                int level      = (unsigned int)(total_thrive / (w*h) * 10);
                
                std::cout << " Total thrive   = " << total_thrive << std::endl;
                std::cout << " Average thrive = " << total_thrive / (w*h) << std::endl;
                std::cout << " LEVELD         = " << (total_thrive / (w*h) * 10) << std::endl;
                std::cout << " LEVEL          = " << level << std::endl;
                
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

  std::cout << "Removing zone building (" << id << ") at " << b->getPosition() << std::endl;

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

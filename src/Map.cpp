#include "Map.hpp"

#include "shared.h"
#include "Tile.hpp"
#include "SimultyException.hpp"

Tile *Map::getTile(unsigned int x, unsigned int y) {

  if(x < 0 || y < 0 || x >= getWidth() || y >= getHeight())
    throw SIMULTYEXCEPTION("Out of bounds");
  else
    return &tiles[y * width + x];

}
Tile *Map::getTile(Point p) {
  return getTile(p.getX(), p.getY());
}

unsigned int Map::getWidth() {
    return width;
}
unsigned int Map::getHeight() {
    return height;
}

void Map::buyLand(unsigned char owner, Point from, Point to) {
  for(unsigned int x = from.getX(); x <= (unsigned int)to.getX(); x++) {
    for(unsigned int y = from.getY(); y <= (unsigned int)to.getY(); y++) {
      if(getTile(x, y)->getOwner() == -1) {
        getTile(x, y)->setOwner(owner);
      }
    }
  }
}
int  Map::buyLandCost(unsigned char owner, Point from, Point to) {
  int cost = 0;
  for(unsigned int x = from.getX(); x <= (unsigned int)to.getX(); x++) {
    for(unsigned int y = from.getY(); y <= (unsigned int)to.getY(); y++) {
      if(getTile(x, y)->getOwner() == -1) {
        cost += SIMULTY_COST_LAND;
      }
    }
  }
  return cost;
}


void Map::bulldoze(unsigned char owner, Point from, Point to) {

  // TODO, check for valid points

  for(unsigned int x = from.getX(); x <= (unsigned int)to.getX(); x++) {
    for(unsigned int y = from.getY(); y <= (unsigned int)to.getY(); y++) {
      if(getTile(x, y)->getOwner() == owner) {
        getTile(x, y)->setRoad(false);
        getTile(x, y)->setZone(0);
      }
    }
  }
}

int Map::bulldozeCost(unsigned char owner, Point from, Point to) {
  int cost = 0;
  for(unsigned int x = from.getX(); x <= (unsigned int)to.getX(); x++) {
    for(unsigned int y = from.getY(); y <= (unsigned int)to.getY(); y++) {
      if(getTile(x, y)->getOwner() == owner) {
        if(getTile(x, y)->isRoad())cost += 10;
        if(getTile(x, y)->getZone() != 0)cost += 5;
      }
    }
  }
  return cost;
}

void Map::buildRoad(unsigned char owner, Point from, Point to) {
  for(unsigned int x = from.getX(); x <= (unsigned int)to.getX(); x++) {
    for(unsigned int y = from.getY(); y <= (unsigned int)to.getY(); y++) {
      if(getTile(x, y)->isRoad() == false && getTile(x, y)->getZone() == 0) {
        getTile(x, y)->setRoad(true);
      }
    }
  }
}
int Map::buildRoadCost(unsigned char owner, Point from, Point to) {
  int cost = 0;
  
  for(unsigned int x = from.getX(); x <= (unsigned int)to.getX(); x++) {
    for(unsigned int y = from.getY(); y <= (unsigned int)to.getY(); y++) {
      if(getTile(x, y)->isRoad() == false && getTile(x, y)->getZone() == 0) {
        cost += SIMULTY_COST_ROAD;
      }
    }
  }
  return cost;

}
    
void Map::buildZone(unsigned char owner, Point from, Point to) {

}
int  Map::buildZoneCost(unsigned char owner, Point from, Point to) {
  return 0;
}


Map::Map(unsigned int height, unsigned int width) {

  this->width  = height;
  this->height = width;

  for(unsigned int x = 0; x < width; x++) {
    for(unsigned int y = 0; y < height; y++) {
        Tile tile;
        tiles.push_back(tile);
    }
  }
}

Map::~Map()
{


}



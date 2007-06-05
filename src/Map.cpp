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

void Map::buildZone(unsigned char owner, int type, Point from, Point to) {
  for(unsigned int x = from.getX(); x <= (unsigned int)from.getY(); x++)
    for(unsigned int y = to.getX(); y <= (unsigned int)to.getY(); y++)
      if(getTile(x, y)->getOwner() == owner && getTile(x,y)->getZone() == 0) {
        getTile(x,y)->setZone(type);
      }
}
int  Map::buildZoneCost(unsigned char owner, int type, Point from, Point to) {
  int cost = 0;
  int cost_per_tile;
  switch(type) {
    case SIMULTY_ZONE_COM:
      cost_per_tile = SIMULTY_COST_COM;
      break;
    case SIMULTY_ZONE_IND:
      cost_per_tile = SIMULTY_COST_IND;
      break;
    case SIMULTY_ZONE_RES:
      cost_per_tile = SIMULTY_COST_RES;
      break;
    default:
      throw SIMULTYEXCEPTION("Unknown zone type");
  }

  for(unsigned int x = from.getX(); x <= (unsigned int)from.getY(); x++)
    for(unsigned int y = to.getX(); y <= (unsigned int)to.getY(); y++)
      if(getTile(x, y)->getOwner() == owner && getTile(x,y)->getZone() == 0) {
        cost += cost_per_tile;
      }

  return cost;
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

Map::~Map() {
}

unsigned char Map::getAdjacentRoads(Point at) {
  unsigned char maskMe = 0;
  if(getTile(at.getX(),   at.getY()+1))
    maskMe |= ROAD_UP;

  if(getTile(at.getX()+1, at.getY()))
    maskMe |= ROAD_RIGHT;

  if(getTile(at.getX(),   at.getY()-1))
    maskMe |= ROAD_DOWN;

  if(getTile(at.getX()-1, at.getY()))
    maskMe |= ROAD_LEFT;

  return maskMe;
}

/*bool Map::isConnectedToZone(Point start, unsigned char zone) {

  for(int x = start.getX()-3; x < start.getX()+3; x++) {
    for(int y = start.getY()-3; y < start.getY()+3; y++) {
      if(getTile(x, y)->isRoad()) {
          if(walkRoad(Point road, DIR_NONE, unsigned char zone))
            return true;
        }
      }
    }
  }
}*/

bool Map::walkRoad(Point road, unsigned char direction, bool lookFor(Point)) {

  if(lookFor(road) == true)
    return true;

  unsigned char maskedRoads = getAdjacentRoads(road);

  bool roads[4] = {
    maskedRoads & ROAD_UP     == ROAD_UP,
    maskedRoads & ROAD_RIGHT  == ROAD_RIGHT,
    maskedRoads & ROAD_DOWN   == ROAD_DOWN,
    maskedRoads & ROAD_LEFT   == ROAD_LEFT
  };

  if(!lookFor(road)) {

    if(roads[DIR_UP] == false && roads[DIR_RIGHT] == false && roads[DIR_DOWN] == false && roads[DIR_LEFT] == false)
      return false;

    switch(direction) {
      case DIR_UP:
        return walkRoad(Point(road.getX(), road.getY()+1), DIR_UP, lookFor);
        break;
      case DIR_RIGHT:
        return walkRoad(Point(road.getX()+1, road.getY()), DIR_RIGHT, lookFor);
        break;
      case DIR_DOWN:
        return walkRoad(Point(road.getX(), road.getY()-1), DIR_DOWN, lookFor);
        break;
      case DIR_LEFT:
        return walkRoad(Point(road.getX()-1, road.getY()), DIR_LEFT, lookFor);
        break;
      case DIR_NONE:
        return (walkRoad(Point(road.getX(),   road.getY()+1), DIR_UP,    lookFor)
             || walkRoad(Point(road.getX()+1, road.getY()),   DIR_RIGHT, lookFor)
             || walkRoad(Point(road.getX(),   road.getY()-1), DIR_DOWN,  lookFor)
             || walkRoad(Point(road.getX()-1, road.getY()),   DIR_LEFT,  lookFor));
        break;
      default:
        return false;
        break;
    }
  }
}


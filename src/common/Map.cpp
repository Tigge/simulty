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

  Point::fixOrder(from, to);

  for(unsigned int x = from.getX(); x <= (unsigned int)to.getX(); x++) {
    for(unsigned int y = from.getY(); y <= (unsigned int)to.getY(); y++) {
      if(!outOfBounds(Point(x, y))) {
        if(getTile(x, y)->getOwner() == -1) {
          getTile(x, y)->setOwner(owner);
        }
      }
    }
  }
}
int  Map::buyLandCost(unsigned char owner, Point from, Point to) {

  from.unsign();
  to.unsign();
  Point::fixOrder(from, to);

  int cost = 0;
  for(unsigned int x = from.getX(); x <= (unsigned int)to.getX(); x++) {
    for(unsigned int y = from.getY(); y <= (unsigned int)to.getY(); y++) {
      if(!outOfBounds(Point(x, y))) {
        if(getTile(x, y)->getOwner() == -1) {
          cost += SIMULTY_COST_LAND;
        }
      }
    }
  }
  return cost;
}


void Map::bulldoze(unsigned char owner, Point from, Point to) {

  // TODO, check for valid points
  Point::fixOrder(from, to);

  for(unsigned int x = from.getX(); x <= (unsigned int)to.getX(); x++) {
    for(unsigned int y = from.getY(); y <= (unsigned int)to.getY(); y++) {
      if(!outOfBounds(Point(x, y))) {
        if(getTile(x, y)->getOwner() == owner) {
          getTile(x, y)->setRoad(false);
        }
      }
    }
  }
}

int Map::bulldozeCost(unsigned char owner, Point from, Point to) {

  from.unsign();
  to.unsign();
  Point::fixOrder(from, to);

  int cost = 0;
  for(unsigned int x = from.getX(); x <= (unsigned int)to.getX(); x++) {
    for(unsigned int y = from.getY(); y <= (unsigned int)to.getY(); y++) {
      if(!outOfBounds(Point(x, y))) {
        if(getTile(x, y)->getOwner() == owner) {
          if(getTile(x, y)->isRoad())cost += 10;
        }
      }
    }
  }
  return cost;
}

void Map::buildRoad(unsigned char owner, Point from, Point to) {

  // TODO: XXX, fix more general
  Point line;

  if((from.getX() - to.getX())*(from.getX() - to.getX() < 0 ? -1 : 1)
   > (from.getY() - to.getY())*(from.getY() - to.getY() < 0 ? -1 : 1)) {  // delta x > delta y
    line.setY(from.getY());
    line.setX(to.getX());
  }
  else {
    line.setX(from.getX());
    line.setY(to.getY());
  }

  Point::fixOrder(from, to);

  // Horizontal
  for(unsigned int x = from.getX(); x <= (unsigned int)to.getX(); x++) {
    if(!outOfBounds(Point(x, line.getY()))) {
      if(getTile(x, line.getY())->isRoad() == false
      && getTile(x, line.getY())->getZone() == 0
      && getTile(x, line.getY())->getOwner() == owner) {
        getTile(x, line.getY())->setRoad(true);
      }
    }
  }

  // Vertical
  for(unsigned int y = from.getY(); y <= (unsigned int)to.getY(); y++) {
    if(!outOfBounds(Point(line.getX(), y))) {
      if(getTile(line.getX(), y)->isRoad() == false
      && getTile(line.getX(), y)->getZone() == 0
      && getTile(line.getX(), y)->getOwner() == owner) {
        getTile(line.getX(), y)->setRoad(true);
      }
    }
  }
}
int Map::buildRoadCost(unsigned char owner, Point from, Point to) {

  Point line;
  int cost = 0;

  if((from.getX() - to.getX())*(from.getX() - to.getX() < 0 ? -1 : 1)
   > (from.getY() - to.getY())*(from.getY() - to.getY() < 0 ? -1 : 1)) {  // delta x > delta y
    line.setY(from.getY());
    line.setX(to.getX());
  }
  else {
    line.setX(from.getX());
    line.setY(to.getY());
  }

  Point::fixOrder(from, to);

  // Horizontal
  for(unsigned int x = from.getX(); x <= (unsigned int)to.getX(); x++) {
    if(!outOfBounds(Point(x, line.getY()))) {
      if(getTile(x, line.getY())->isRoad() == false
      && getTile(x, line.getY())->getZone() == 0
      && getTile(x, line.getY())->getOwner() == owner) {
        cost += SIMULTY_COST_ROAD;
      }
    }
  }

  // Vertical
  for(unsigned int y = from.getY() + 1; y <= (unsigned int)to.getY(); y++) {
    if(!outOfBounds(Point(line.getX(), y))) {
      if(getTile(line.getX(), y)->isRoad() == false
      && getTile(line.getX(), y)->getZone() == 0
      && getTile(line.getX(), y)->getOwner() == owner) {
        cost += SIMULTY_COST_ROAD;
      }
    }
  }

  return cost;
}

void Map::buildZone(unsigned char owner, int type, Point from, Point to) {

  Point::fixOrder(from, to);

  for(unsigned int x = from.getX(); x <= (unsigned int)to.getX(); x++) {
    for(unsigned int y = from.getY(); y <= (unsigned int)to.getY(); y++) {
    if(!outOfBounds(Point(x, y))) {
      if(getTile(x, y)->getOwner() == owner && !getTile(x,y)->isOccupied()) {
          getTile(x,y)->setZone(type);
        }
      }
    }
  }
}

int  Map::buildZoneCost(unsigned char owner, int type, Point from, Point to) {

  from.unsign();
  to.unsign();
  Point::fixOrder(from, to);

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

  for(unsigned int x = from.getX(); x <= (unsigned int)to.getX(); x++) {
    for(unsigned int y = from.getY(); y <= (unsigned int)to.getY(); y++) {
      if(!outOfBounds(Point(x, y))) {
        if(getTile(x, y)->getOwner() == owner && !getTile(x,y)->isOccupied()) {
          cost += cost_per_tile;
        }
      }
    }
  }

  return cost;
}

void Map::deZone(unsigned char owner, Point from, Point to) {

  Point::fixOrder(from, to);

  for(int x = from.getX(); x <= to.getX(); x++) {
    for(int y = from.getY(); y <= to.getY(); y++) {
      if(getTile(x, y)->getOwner() == owner 
          && getTile(x, y)->getBuilding() == SIMULTY_BUILDING_NONE) {
        if(getTile(x, y)->getZone() != SIMULTY_ZONE_NONE) {
          getTile(x, y)->setZone(SIMULTY_ZONE_NONE);
        }
      }
    }
  }
}

int Map::deZoneCost(unsigned char owner, Point from, Point to) {

  Point::fixOrder(from, to);

  int cost = 0;
  for(int x = from.getX(); x <= to.getX(); x++) {
    for(int y = from.getY(); y <= to.getY(); y++) {
      if(getTile(x, y)->getOwner() == owner 
          && getTile(x, y)->getBuilding() == SIMULTY_BUILDING_NONE) {
        if(getTile(x, y)->getZone() == SIMULTY_ZONE_RES)cost -= SIMULTY_COST_RES / 2;
        if(getTile(x, y)->getZone() == SIMULTY_ZONE_COM)cost -= SIMULTY_COST_COM / 2;
        if(getTile(x, y)->getZone() == SIMULTY_ZONE_IND)cost -= SIMULTY_COST_IND / 2;
      }
    }
  }
  return cost;
}

void Map::markBuilding(Building *b, int type) {

  for(int x = b->getPosition().getX(); x < b->getPosition().getX() + b->getWidth(); x++) {
    for(int y = b->getPosition().getY(); y < b->getPosition().getY() + b->getHeight(); y++) {
      getTile(x, y)->setBuilding(type);
    }
  }

}

void Map::unmarkBuilding(Building *b) {

  for(int x = b->getPosition().getX(); x < b->getPosition().getX() + b->getWidth(); x++) {
    for(int y = b->getPosition().getY(); y < b->getPosition().getY() + b->getHeight(); y++) {
      getTile(x, y)->setBuilding(SIMULTY_BUILDING_NONE);
    }
  }

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

  if(!outOfBounds(Point(at.getX(), at.getY()+1)) && getTile(at.getX(), at.getY()+1)->isRoad())
    maskMe |= ROAD_UP;

  if(!outOfBounds(Point(at.getX()+1, at.getY())) && getTile(at.getX()+1, at.getY())->isRoad())
    maskMe |= ROAD_RIGHT;

  if(!outOfBounds(Point(at.getX(), at.getY()-1)) && getTile(at.getX(), at.getY()-1)->isRoad())
    maskMe |= ROAD_DOWN;

  if(!outOfBounds(Point(at.getX()-1, at.getY())) && getTile(at.getX()-1, at.getY())->isRoad())
    maskMe |= ROAD_LEFT;

  return maskMe;
}

bool Map::isCloseToRoad(Point p) {

  const int ROAD_DISTANCE_LIMIT = 3;
  for(int x = p.getX() - ROAD_DISTANCE_LIMIT; x < p.getX() + ROAD_DISTANCE_LIMIT; x++) {
    for(int y = p.getY() - ROAD_DISTANCE_LIMIT; y < p.getY() + ROAD_DISTANCE_LIMIT; y++) {
      if(!outOfBounds(Point(x, y)) && getTile(x, y)->isRoad()) {
        return true;
      }
    }
  }
  return false;
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

  /*char DIR_UP_RIGHT_DOWN   = 5,
       DIR_UP_RIGHT_LEFT   = 6,
       DIR_UP_DOWN_LEFT    = 6,
       DIR_UP_RIGHT        = 7;
       DIR_UP_DOWN         = 8;
       DIR_UP_LEFT         = 9;
       DIR_RIGHT_DOWN_LEFT = 10;
       DIR_RIGHT_DOWN      = 11;
       DIR_RIGHT_LEFT      = 12;
       DIR_DOWN_LEFT       = 13;*/

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

    if(!roads[direction]) {

      if(roads[DIR_UP]) {
        if(roads[DIR_RIGHT]) {
          if(roads[DIR_DOWN]) {
            return (walkRoad(Point(road.getX(),   road.getY()+1), DIR_UP,    lookFor)
                 || walkRoad(Point(road.getX()+1, road.getY()),   DIR_RIGHT, lookFor)
                 || walkRoad(Point(road.getX(),   road.getY()-1), DIR_DOWN,  lookFor));
          }
          else if(roads[DIR_LEFT]) {
            return (walkRoad(Point(road.getX(),   road.getY()+1), DIR_UP,    lookFor)
                 || walkRoad(Point(road.getX()+1, road.getY()),   DIR_RIGHT, lookFor)
                 || walkRoad(Point(road.getX()-1, road.getY()),   DIR_LEFT,  lookFor));
          }
        }
        else if(roads[DIR_DOWN]) {
          if(roads[DIR_LEFT]) {
            return (walkRoad(Point(road.getX(),   road.getY()+1), DIR_UP,    lookFor)
                 || walkRoad(Point(road.getX(),   road.getY()-1), DIR_DOWN,  lookFor)
                 || walkRoad(Point(road.getX()-1, road.getY()),   DIR_LEFT,  lookFor));
          }
        }
        else if(roads[DIR_LEFT]) {
          return (walkRoad(Point(road.getX(),   road.getY()+1), DIR_UP,    lookFor)
               || walkRoad(Point(road.getX()+1, road.getY()),   DIR_RIGHT, lookFor)
               || walkRoad(Point(road.getX(),   road.getY()-1), DIR_DOWN,  lookFor)
               || walkRoad(Point(road.getX()-1, road.getY()),   DIR_LEFT,  lookFor));
        }
        else {
          return (walkRoad(Point(road.getX(),   road.getY()+1), DIR_UP,    lookFor)
               || walkRoad(Point(road.getX()+1, road.getY()),   DIR_RIGHT, lookFor));
        }
      }

      else if(roads[DIR_RIGHT]) {
        if(roads[DIR_DOWN]) {
          if(roads[DIR_LEFT]) {
            return (walkRoad(Point(road.getX()+1, road.getY()),   DIR_RIGHT, lookFor)
                 || walkRoad(Point(road.getX(),   road.getY()-1), DIR_DOWN,  lookFor)
                 || walkRoad(Point(road.getX()-1, road.getY()),   DIR_LEFT,  lookFor));
          }
          else {
            return (walkRoad(Point(road.getX()+1, road.getY()),   DIR_RIGHT, lookFor)
                 || walkRoad(Point(road.getX(),   road.getY()-1), DIR_DOWN,  lookFor));
          }
        }
        else if(roads[DIR_LEFT]) {
          return (walkRoad(Point(road.getX()+1, road.getY()),   DIR_RIGHT, lookFor)
               || walkRoad(Point(road.getX()-1, road.getY()),   DIR_LEFT,  lookFor));
        }
        else {
          return walkRoad(Point(road.getX()+1, road.getY()),   DIR_RIGHT, lookFor);
        }
      }

      else if(roads[DIR_DOWN]) {
        if(roads[DIR_LEFT]) {
          return (walkRoad(Point(road.getX(),   road.getY()-1), DIR_DOWN,  lookFor)
               || walkRoad(Point(road.getX()-1, road.getY()),   DIR_LEFT,  lookFor));
        }
      }

      else {  // Genuine left turn
        return walkRoad(Point(road.getX()-1, road.getY()),   DIR_LEFT,  lookFor);
      }
    }

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
        throw "You so drunk you can't even walk straight! (Map::walkRoad)";
        break;
    }
  }

  return true; // We found what we looked for!
}


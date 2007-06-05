#ifndef _MAP_HPP_
#define _MAP_HPP_

#define TILE_W 64
#define TILE_H 32

#include "Tile.hpp"
#include "Point.hpp"

#include <vector>

const unsigned char ROAD_UP     = 1;
const unsigned char ROAD_RIGHT  = 2;
const unsigned char ROAD_DOWN   = 4;
const unsigned char ROAD_LEFT   = 8;


class Map {
  // Attributes
  private:
    unsigned int width;
    unsigned int height;

    std::vector<Tile> tiles;

  // Operations
  public:

    Map(unsigned int width, unsigned int height);
    virtual ~Map();

    virtual Tile *getTile(unsigned int x, unsigned int y);
    virtual Tile *getTile(Point p);

    virtual unsigned int getWidth();
    virtual unsigned int getHeight();

    virtual void buyLand(unsigned char owner, Point from, Point to);
    virtual int  buyLandCost(unsigned char owner, Point from, Point to);

    virtual void bulldoze(unsigned char owner, Point from, Point to);
    virtual int  bulldozeCost(unsigned char owner, Point from, Point to);

    virtual void buildRoad(unsigned char owner, Point from, Point to);
    virtual int  buildRoadCost(unsigned char owner, Point from, Point to);

    virtual void buildZone(unsigned char owner, Point from, Point to);
    virtual int  buildZoneCost(unsigned char owner, Point from, Point to);

    virtual unsigned char getAdjacentRoads(Point at);
    //virtual bool isConnectedToZone(Point start, unsigned char zone);
    virtual bool walkRoad(Point road, unsigned char direction, bool lookFor(Point));

};

#endif

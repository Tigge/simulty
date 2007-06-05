#ifndef _MAP_HPP_
#define _MAP_HPP_

#define TILE_W 64
#define TILE_H 32

#include "Tile.hpp"
#include "Point.hpp"

#include <vector>


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

};

#endif

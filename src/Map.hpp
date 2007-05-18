#ifndef _MAP_HPP_
#define _MAP_HPP_

#define TILE_W 64
#define TILE_H 32

#include "Tile.hpp"

#include <vector>


class Map {
  // Attributes
  private:
    int width;
    int height;

    std::vector<Tile> tiles;

  // Operations
  public:
  
    Map(int width, int height);
    virtual ~Map();

    virtual Tile *getTile(int x, int y);    
    
    virtual int getWidth();
    virtual int getHeight();

};

#endif

#ifndef _MAP_BASE_H_
#define _MAP_BASE_H_

#define TILE_W 64
#define TILE_H 32

#include "tile.h"

#include <vector>

using namespace std;

class map_base {
  // Attributes
  public:
    int width;
    int height;

    vector<tile> tiles;

  // Operations
  public:
  
    tile get(int x, int y);
  
    map_base (  );
    ~map_base (  );
};

#endif

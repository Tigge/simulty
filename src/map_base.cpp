
#include "map.h"
#include "tile.h"


tile map_base::get(int x, int y) {

  return tiles[y * width + x];

}

map_base::map_base (  ){

    width  = 30;
    height = 30;

    for(int x = 0; x < width; x++)
        for(int y = 0; y < height; y++)
        {
            tile tmp; 
            //if(y % 2)tmp.road = true;
            tiles.push_back(tmp);
        }

}

map_base::~map_base()
{


}



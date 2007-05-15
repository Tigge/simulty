#ifndef _MAP_H_
#define _MAP_H_

#define TILE_W 64
#define TILE_H 32



#include "allegro.h"

#ifdef WIN32
  #include "winalleg.h"
#endif


#include "map_base.h"
#include "tile.h"

#include "Point.hpp"

#include <vector>



class map : public map_base {

  private:

    BITMAP *t_owned;
    
    BITMAP *t_zone_res;
    BITMAP *t_zone_com;
    BITMAP *t_zone_ind;
    
    BITMAP **t_border;

    BITMAP **t_terrain;
    BITMAP **t_roads;

    BITMAP *mouse_hint;

    BITMAP *road_tile(int x, int y);
    void clean_bitmap(BITMAP *);

  // Operations
  public:


    Point val2tile(Point input);
    Point val2tile_real(Point input);

    Point tile2val(Point input);

    Point pos_to_tile(Point pos, Point cam);
    Point tile_to_pos(Point tile, int dir, Point cam);

    map (  );
    ~map (  );
    void render (BITMAP *b, Point cam);
};

#endif

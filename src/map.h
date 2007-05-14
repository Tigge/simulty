#ifndef _MAP_H_
#define _MAP_H_

#define TILE_W 64
#define TILE_H 32



#include "allegro.h"

#include "map_base.h"
#include "tile.h"

#include "point.h"

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


    point val2tile(point input);
    point val2tile_real(point input);

    point tile2val(point input);

    point pos_to_tile(point pos, point cam);
    point tile_to_pos(point tile, int dir, point cam);

    map (  );
    ~map (  );
    void render (BITMAP *b, point cam);
};

#endif

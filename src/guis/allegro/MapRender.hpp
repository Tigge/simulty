#ifndef _MAPRENDER_HPP_
#define _MAPRENDER_HPP_

#include "allegro.h"
#ifdef WIN32
  #include "winalleg.h"
#endif

#include "../../shared.h"

#include "../../Map.hpp"
#include "../../Point.hpp"
#include "Camera.hpp"

#include "ImageLoader.hpp"


class MapRender {


  private:

    Map    *m;

    BITMAP *t_owned;
    
    BITMAP *t_zone_res;
    BITMAP *t_zone_com;
    BITMAP *t_zone_ind;
    
    BITMAP **t_border;

    BITMAP **t_terrain;
    BITMAP **t_roads;

    BITMAP *mouse_hint;

    BITMAP *getRoadBitmap(int x, int y);

  // Operations
  public:
    

    Point toTileCoord(Point screenCoord);
    Point toTileCoord(Point screenCoord, Camera cam);    
    Point toScreenCoord(Point tileCoord);
    Point toScreenCoord(Point tileCoord, Camera cam);

    Map *getMap();
    void setMap(Map *);

    MapRender();
    ~MapRender();
    void render (BITMAP *b, Camera cam);


};

#endif

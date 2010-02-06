#ifndef _MAPRENDER_HPP_
#define _MAPRENDER_HPP_

#include "../../shared.h"

#include "../../Map.hpp"
#include "../../Point.hpp"
#include "Camera.hpp"

#include "ImageLoader.hpp"

#include "SDL.h"


class MapRender {


  private:

    Map    *m;

    SDL_Surface *t_owned;
    
    SDL_Surface *t_zone_res;
    SDL_Surface *t_zone_com;
    SDL_Surface *t_zone_ind;
    
    SDL_Surface **t_border;

    SDL_Surface **t_terrain;
    SDL_Surface **t_roads;

    SDL_Surface *mouse_hint;

    SDL_Surface *getRoadBitmap(int x, int y);

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
    void render (SDL_Surface *b, Camera cam);


};

#endif

#ifndef _MINIMAP_HPP_
#define _MINIMAP_HPP_

#include "allegro.h"
#ifdef WIN32
  #include "winalleg.h"
#endif

#include "../../Map.hpp"
#include "../../Point.hpp"

class MiniMap {

  private:

  int mode;
  Map *map;

  public:
  
  MiniMap(Map *);
  ~MiniMap();
  
  void render(BITMAP *bitmap, Point where);
  
  int getWidth();
  int getHeight();

};

#endif

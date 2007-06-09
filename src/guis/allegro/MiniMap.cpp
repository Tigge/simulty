#include "MiniMap.hpp"



MiniMap::MiniMap(Map *map) {
  this->map = map;
}
MiniMap::~MiniMap() {

}

int MiniMap::getWidth() {
  return map->getWidth() * 2 + map->getHeight() * 2 - 2;
}
int MiniMap::getHeight() {
  return map->getWidth() + map->getHeight() - 1;
}

void MiniMap::render(BITMAP *bitmap, Point where) {

  Point base = where + Point(map->getWidth() * 2, 0);

  for(unsigned int x = 0; x < map->getWidth(); x++) {
    for(unsigned int y = 0; y < map->getHeight(); y++) {
      Tile *t = map->getTile(x, y);
      
      int color;
      
      if(t->isRoad()) {
        color = makecol(100, 100, 100);
      } else if(t->getZone() == SIMULTY_ZONE_RES) {
        color = makecol(78, 216, 24);
      } else if(t->getZone() == SIMULTY_ZONE_COM) {
        color = makecol(113, 148, 227);
      } else if(t->getZone() == SIMULTY_ZONE_IND) {
        color = makecol(222, 159, 49);
      } else {
        color = makecol(112, 152, 40);
      }
      int bx = base.getX() + x + (y * 2);
      int by = base.getY() + y;

      putpixel(bitmap, bx, by, color);
      putpixel(bitmap, bx + 1, by, color);
      
      if(x != 0) {
        putpixel(bitmap, bx, by - 1, color);
      } if( x != map->getWidth() - 1) {
        putpixel(bitmap, bx + 1, by + 1, color);
      }

    }
    base.translate(-3, 1);
  }
}



#include "Map.hpp"
#include "Tile.hpp"
#include "SimultyException.hpp"

Tile *Map::getTile(unsigned int x, unsigned int y) {

  if(x < 0 || y < 0 || x >= getWidth() || y >= getHeight())
    throw SIMULTYEXCEPTION("Out of bounds");
  else
    return &tiles[y * width + x];

}
Tile *Map::getTile(Point p) {
  return getTile(p.getX(), p.getY());
}

unsigned int Map::getWidth() {
    return width;
}
unsigned int Map::getHeight() {
    return height;
}

Map::Map(unsigned int height, unsigned int width) {

  this->width  = height;
  this->height = width;

  for(unsigned int x = 0; x < width; x++) {
    for(unsigned int y = 0; y < height; y++) {
        Tile tile;
        tiles.push_back(tile);
    }
  }
}

Map::~Map()
{


}




#include "Map.hpp"
#include "Tile.hpp"


Tile *Map::getTile(int x, int y) {

  return &tiles[y * width + x];

}

int Map::getWidth() {
    return width;
}
int Map::getHeight() {
    return height;
}

Map::Map(int height, int width) {

    this->width  = height;
    this->height = width;

    for(int x = 0; x < width; x++)
        for(int y = 0; y < height; y++)
        {
            Tile tile;
            tiles.push_back(tile);
        }

}

Map::~Map()
{


}



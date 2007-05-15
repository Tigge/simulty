#include "Tile.h"

Tile::Tile (  ){


    owner = -1;
    terrain = 0;
    zone = 0;
    house = 0;

    road = false;

}

Tile::~Tile (  ){
}


char Tile::getOwner() {
    return owner;
}

char Tile::getTerrain() {
    return terrain;
}
char Tile::getZone() {
    return zone;
}

char Tile::getHouse() {
    return house;
}
    
bool Tile::isRoad() {
    return road;
}



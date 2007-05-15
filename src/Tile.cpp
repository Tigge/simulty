#include "Tile.hpp"

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

void Tile::setOwner(char owner) {
    this->owner = owner;
}

char Tile::getTerrain() {
    return terrain;
}
char Tile::getZone() {
    return zone;
}
void Tile::setZone(unsigned char zone) {
    this->zone = zone;
}

char Tile::getHouse() {
    return house;
}
    
bool Tile::isRoad() {
    return road;
}

void Tile::setRoad(bool road) {
    this->road = road;
}


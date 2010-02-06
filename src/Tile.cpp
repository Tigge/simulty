#include "Tile.hpp"

#include <iostream>

Tile::Tile (  ){

  owner    = -1;
  terrain  = SIMULTY_TERRAIN_GRASS;
  zone     = SIMULTY_ZONE_NONE;
  building = SIMULTY_BUILDING_NONE;

  road     = false;

}

Tile::~Tile (  ){
}

char Tile::getOwner() {
  return owner;
}

void Tile::setOwner(char owner) {
  this->owner = owner;
}

unsigned char Tile::getTerrain() {
  return terrain;
}
unsigned char Tile::getZone() {
  return zone;
}
void Tile::setZone(unsigned char zone) {
  this->zone = zone;
}

unsigned char Tile::getBuilding() {
  return building;
}

void Tile::setBuilding(unsigned char building) {
  this->building = building;
}

bool Tile::isRoad() {
  return road;
}

void Tile::setRoad(bool road) {
  this->road = road;
}

bool Tile::isOccupied() {
  return this->road || building != SIMULTY_BUILDING_NONE || zone != SIMULTY_ZONE_NONE;
}


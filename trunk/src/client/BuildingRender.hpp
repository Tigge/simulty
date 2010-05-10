#ifndef _BUILDINGRENDER_HPP_
#define _BUILDINGRENDER_HPP_

#include "Building.hpp"
#include "BuildingZone.hpp"
#include "BuildingManager.hpp"

#include "Point.hpp"
#include "Camera.hpp"

#include "MapRender.hpp"

class BuildingRender {

  private:

  SDL_Surface *buildingPolice;
  SDL_Surface *buildingFire;
  SDL_Surface *buildingHospital;
  SDL_Surface *buildingResidential;

  SDL_Surface *buildingHouse1x1;
  SDL_Surface *buildingHouse1x2;
  SDL_Surface *buildingHouse1x3;
  SDL_Surface *buildingHouse2x1;
  SDL_Surface *buildingHouse2x2;
  SDL_Surface *buildingHouse2x3;
  SDL_Surface *buildingHouse3x1;
  SDL_Surface *buildingHouse3x2;
  SDL_Surface *buildingHouse3x3;

  public:

  BuildingRender();
  ~BuildingRender();

  SDL_Surface *getZoneBuilding(int w, int h);
  void         renderBuilding(SDL_Surface *r, Building *b, Point where);
  void         render(SDL_Surface *r, MapRender *m, Camera cam, BuildingManager *bm);


};


#endif

#ifndef _BUILDINGRENDER_HPP_
#define _BUILDINGRENDER_HPP_

#include "../../Building.hpp"
#include "../../BuildingZone.hpp"
#include "../../BuildingManager.hpp"

#include "../../Point.hpp"
#include "Camera.hpp"

#include "MapRender.hpp"

class BuildingRender {

  private:

  SDL_Surface *buildingPolice;
  SDL_Surface *buildingFire;
  SDL_Surface *buildingHospital;
  SDL_Surface *buildingResidential;


  public:

  BuildingRender();
  ~BuildingRender();

  void renderBuilding(SDL_Surface *r, Building *b, Point where);
  void render(SDL_Surface *r, MapRender *m, Camera cam, BuildingManager *bm);


};


#endif

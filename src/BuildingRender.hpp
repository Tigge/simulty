#ifndef _BUILDINGRENDER_HPP_
#define _BUILDINGRENDER_HPP_

#include "allegro.h"
#ifdef WIN32
  #include "winalleg.h"
#endif

#include "Building.hpp"
#include "BuildingManager.hpp"

#include "Point.hpp"
#include "Camera.hpp"

#include "MapRender.hpp"

class BuildingRender {

    private:

    BITMAP *buildingPolice;
    BITMAP *buildingFire;
    BITMAP *buildingHospital;
    BITMAP *buildingResidential;


    public:

    BuildingRender();

    void renderBuilding(BITMAP *r, Building *b, Point where);
    void render(BITMAP *r, MapRender *m, Camera cam, BuildingManager *bm);


};


#endif

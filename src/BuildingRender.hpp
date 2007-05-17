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

#include "GUIMap.hpp"

class BuildingRender {


    private:
    
    BITMAP *buildingPolice;
    BITMAP *buildingFire;
    BITMAP *buildingHospital;

    
    public:
    
    BuildingRender();    
    
    void renderBuilding(BITMAP *r, Building *b, Point where);
    void render(BITMAP *r, GUIMap *m, Camera cam, BuildingManager *bm);


};


#endif

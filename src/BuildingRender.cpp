
#include "BuildingRender.hpp"

#include <iostream>
    
BuildingRender::BuildingRender() {

  try {

    buildingPolice   = load_bitmap("img/special/police.pcx",   NULL);
    buildingFire     = load_bitmap("img/special/fire.pcx",     NULL);
    buildingHospital = load_bitmap("img/special/hospital.pcx", NULL);    
    
  } catch(ImageLoaderException e) {
    allegro_message("Error: %s", e.what());
    exit(1);
  }
    
}

void BuildingRender::renderBuilding(BITMAP *r, Building *b, Point where) {

    BITMAP *image = NULL;
    
    switch(b->getType()) {
    
        case Building::TYPE_POLICE:   image = buildingPolice;   break;
        case Building::TYPE_FIRE:     image = buildingFire;     break;
        case Building::TYPE_HOSPITAL: image = buildingHospital; break;
        default: throw -1;
    }
  
    
   // masked_blit(image, r, 0, 0, where.getX(), where.getY(), image->w, image->h);    

    
    int y = where.getY() - image->h + b->getHeight() * TILE_H;
    int x = where.getX() - (b->getWidth() - 1)  * TILE_W / 2;
      
    masked_blit(image, r, 0, 0, x, y, image->w, image->h);

}

    
void BuildingRender::render(BITMAP *r, MapRender *mr, Camera cam, BuildingManager *bm) {

    //Render depth by Z depth (TODO, render only visible)
    for(int d = 0; d < (mr->getMap()->getWidth() + mr->getMap()->getHeight()); d++) {

        for(int i = 0; i < bm->getSpecialBuildingCount(); i++) {

            Building *b = bm->getSpecialBuilding(i);       
            if(b->getZ() == d)
                renderBuilding(r, b, mr->toScreenCoord(b->getPosition(), cam)); 
        }  
    }    

}

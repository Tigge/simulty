
#include "BuildingRender.hpp"

#include <iostream>
    
BuildingRender::BuildingRender() {

    buildingPolice   = load_bitmap("img/special/police.pcx",   NULL);
    buildingFire     = load_bitmap("img/special/fire.pcx",     NULL);
    buildingHospital = load_bitmap("img/special/hospital.pcx", NULL);    
    
}

void BuildingRender::renderBuilding(BITMAP *r, Building *b, Point where) {

    BITMAP *image = NULL;
    
    switch(b->getType()) {
    
        case Building::TYPE_POLICE: image = buildingPolice; break;
        default: image = buildingPolice; break;
    }
  
    
   // masked_blit(image, r, 0, 0, where.getX(), where.getY(), image->w, image->h);    

    
    int y = where.getY() - image->h + b->getHeight() * TILE_H;
    int x = where.getX() - (b->getWidth() - 1)  * TILE_W / 2;
      
    masked_blit(image, r, 0, 0, x, y, image->w, image->h);

}

    
void BuildingRender::render(BITMAP *r, GUIMap *m, Camera cam, BuildingManager *bm) {

    for(int i = 0; i < bm->getSpecialBuildingCount(); i++) {

      Building *b = bm->getSpecialBuilding(i);       
      //std::cout << b->getPosition() << " " << m->toScreenCoord(b->getPosition(), cam) << std::endl;
 
      renderBuilding(r, b, m->toScreenCoord(b->getPosition(), cam)); 
        
    }
    

}

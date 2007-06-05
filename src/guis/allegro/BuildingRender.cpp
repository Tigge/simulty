
#include "BuildingRender.hpp"

#include <iostream>

BuildingRender::BuildingRender() {

  try {

    buildingPolice      = load_bitmap("img/special/police.pcx",   NULL);
    buildingFire        = load_bitmap("img/special/fire.pcx",     NULL);
    buildingHospital    = load_bitmap("img/special/hospital.pcx", NULL);
    buildingResidential = load_bitmap("img/special/res.pcx",      NULL);

  } catch(ImageLoaderException e) {
    allegro_message("Error: %s", e.what());
    exit(1);
  }

}

BuildingRender::~BuildingRender() {
  destroy_bitmap(buildingPolice);
  destroy_bitmap(buildingFire);
  destroy_bitmap(buildingHospital);
  destroy_bitmap(buildingResidential);
}

void BuildingRender::renderBuilding(BITMAP *r, Building *b, Point where) {

  if(b->getType() == Building::TYPE_RESIDENTIAL
      || b->getType() == Building::TYPE_COMMERSIAL
      || b->getType() == Building::TYPE_INDUSTRIAL) {
    
    int color = makecol(255, 255, 255);
    
    switch(b->getType()) {
      case Building::TYPE_RESIDENTIAL: color = makecol(100, 255, 100); break;
      case Building::TYPE_COMMERSIAL:  color = makecol(100, 100, 255); break;
      case Building::TYPE_INDUSTRIAL:  color = makecol(255, 100, 100); break;
    }
    
    int x = b->getPosition().getX();
    int y = b->getPosition().getY();
    
    int points[8] = { x + TILE_W / 2 + 2, y + 2, 
                      x + (TILE_W / 2) * (b->getWidth() + 1) - 2, y + (TILE_W / 2) * b->getHeight(),
                      x + TILE_W / 2 - 2, y * TILE_H * b->getHeight() - 2,
                      x + (TILE_W / 2) * (b->getWidth() - 1) + 2, y + (TILE_W / 2) * b->getHeight() };
    polygon(r, 4, points, color);

  } else {

    BITMAP *image = NULL;

    switch(b->getType()) {

        case Building::TYPE_POLICE:   image = buildingPolice;   break;
        case Building::TYPE_FIRE:     image = buildingFire;     break;
        case Building::TYPE_HOSPITAL: image = buildingHospital; break;
        case Building::TYPE_RESIDENTIAL: image = buildingResidential; break;
        default: throw "BuildingRender: No image loaded for building type";
    }

    // Calculate position to place image correctly:
    int y = where.getY() - image->h + b->getHeight() * TILE_H;
    int x = where.getX() - (b->getWidth() - 1)  * TILE_W / 2;

    masked_blit(image, r, 0, 0, x, y, image->w, image->h);
  }
}


void BuildingRender::render(BITMAP *r, MapRender *mr, Camera cam, BuildingManager *bm) {

    //Render depth by Z depth (TODO, render only visible)
    for(unsigned int d = 0; d < (mr->getMap()->getWidth() + mr->getMap()->getHeight()); d++) {

        for(unsigned int i = 0; i < bm->getSpecialBuildingCount(); i++) {

            Building *b = bm->getSpecialBuilding(i);
            if(b->getZ() == d)
                renderBuilding(r, b, mr->toScreenCoord(b->getPosition(), cam));
        }
    }

}

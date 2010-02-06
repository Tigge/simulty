
#include "BuildingRender.hpp"

#include <iostream>

BuildingRender::BuildingRender() {

  try {

    buildingPolice      = ImageLoader::getImage("img/special/police.pcx");
    buildingFire        = ImageLoader::getImage("img/special/fire.pcx");
    buildingHospital    = ImageLoader::getImage("img/special/hospital.pcx");
    //buildingResidential = ImageLoader::getImage("img/special/res.pcx");

  } catch(ImageLoaderException e) {
    std::cout << "Error: " << e.what();
    exit(1);
  }

}

BuildingRender::~BuildingRender() {
  SDL_FreeSurface(buildingPolice);
  SDL_FreeSurface(buildingFire);
  SDL_FreeSurface(buildingHospital);
  //SDL_FreeSurface(buildingResidential);
}

void BuildingRender::renderBuilding(SDL_Surface *r, Building *b, Point where) {

  if(b->getType() == Building::TYPE_COMMERSIAL
  || b->getType() == Building::TYPE_INDUSTRIAL
  || b->getType() == Building::TYPE_RESIDENTIAL) {
    /*
    int color = makecol(255, 255, 255);

    switch(b->getType()) {
      case Building::TYPE_RESIDENTIAL: color = makecol(100, 255, 100); break;
      case Building::TYPE_COMMERSIAL:  color = makecol(100, 100, 255); break;
      case Building::TYPE_INDUSTRIAL:  color = makecol(255, 100, 100); break;
    }

    int baseX = where.getX() + TILE_W / 2;
    int baseY = where.getY();
    int addX  = TILE_W / 2;
    int addY  = TILE_H / 2;

    int points[8] = { baseX,
                      baseY + 5,
                      baseX + addX * b->getHeight() - 5,
                      baseY + addY * b->getHeight(),
                      baseX - addX * b->getWidth()+ addX * b->getHeight(),
                      baseY + addY * b->getWidth() + addY * b->getHeight() - 5,
                      baseX - addX * b->getWidth() + 5 ,
                      baseY + addY * b->getWidth()};
    polygon(r, 4, points, color);
    textprintf_ex(r, font, baseX - 10, baseY + 10, makecol(0, 0, 0), -1, "%i,%i", b->getWidth(), b->getHeight());
    
    BuildingZone *z = (BuildingZone *)b;
    textprintf_ex(r, font, baseX - 10, baseY + 16, makecol(0, 0, 0), -1, "%i", z->getLevel());
    */

  } else {

    SDL_Surface *image = NULL;

    switch(b->getType()) {

        case Building::TYPE_POLICE:      image = buildingPolice;   break;
        case Building::TYPE_FIRE:        image = buildingFire;     break;
        case Building::TYPE_HOSPITAL:    image = buildingHospital; break;
        //case Building::TYPE_RESIDENTIAL: image = buildingResidential; break;
        default: throw "BuildingRender: No image loaded for building type";
    }

    // Calculate position to place image correctly:
    int y = where.getY() - image->h + b->getHeight() * TILE_H;
    int x = where.getX() - (b->getWidth() - 1)  * TILE_W / 2;

    SDL_Rect dst = {x, y, image->w, image->h};
    SDL_BlitSurface(image, NULL, r, &dst);
  }
}


void BuildingRender::render(SDL_Surface *r, MapRender *mr, Camera cam, BuildingManager *bm) {

    //Render depth by Z depth (TODO, render only visible)
    for(unsigned int d = 0; d < (mr->getMap()->getWidth() + mr->getMap()->getHeight()); d++) {
        for(unsigned int i = 0; i < bm->getSpecialBuildingCount(); i++) {
            Building *b = bm->getSpecialBuilding(i);
            if(b->getZ() == d)
                renderBuilding(r, b, mr->toScreenCoord(b->getPosition(), cam));
        }
        for(unsigned int i = 0; i < bm->getZoneBuildingCount(); i++) {
            Building *b = bm->getZoneBuilding(i);
            if(b->getZ() == d)
                renderBuilding(r, b, mr->toScreenCoord(b->getPosition(), cam));
        }
    }

}

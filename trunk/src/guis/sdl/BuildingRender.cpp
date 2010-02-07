
#include "BuildingRender.hpp"

#include <iostream>

BuildingRender::BuildingRender() {

  try {

    buildingPolice      = ImageLoader::getImage("img/special/police.pcx");
    buildingFire        = ImageLoader::getImage("img/special/fire.pcx");
    buildingHospital    = ImageLoader::getImage("img/special/hospital.pcx");
    //buildingResidential = ImageLoader::getImage("img/special/res.pcx");

    buildingHouse1x1    = ImageLoader::getImage("img/buildings/house_1x1.png");
    buildingHouse1x2    = ImageLoader::getImage("img/buildings/house_1x2.png");
    buildingHouse1x3    = ImageLoader::getImage("img/buildings/house_1x3.png");
    buildingHouse2x1    = ImageLoader::getImage("img/buildings/house_2x1.png");
    buildingHouse2x2    = ImageLoader::getImage("img/buildings/house_2x2.png");
    buildingHouse2x3    = ImageLoader::getImage("img/buildings/house_2x3.png");
    buildingHouse3x1    = ImageLoader::getImage("img/buildings/house_3x1.png");
    buildingHouse3x2    = ImageLoader::getImage("img/buildings/house_3x2.png");
    buildingHouse3x3    = ImageLoader::getImage("img/buildings/house_3x3.png");

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
  
  SDL_FreeSurface(buildingHouse1x1);
  SDL_FreeSurface(buildingHouse1x2);
  SDL_FreeSurface(buildingHouse1x3);
  SDL_FreeSurface(buildingHouse2x1);
  SDL_FreeSurface(buildingHouse2x2);
  SDL_FreeSurface(buildingHouse2x3);
  SDL_FreeSurface(buildingHouse3x1);
  SDL_FreeSurface(buildingHouse3x2);
  SDL_FreeSurface(buildingHouse3x3);
}

SDL_Surface *BuildingRender::getZoneBuilding(int w, int h) {
  if(w == 1 && h == 1) {
    return buildingHouse1x1;
  } else if(w == 1 && h == 2) {
    return buildingHouse1x2;
  } else if(w == 1 && h == 3) {
    return buildingHouse1x3;
  } else if(w == 2 && h == 1) {
    return buildingHouse2x1;
  } else if(w == 2 && h == 2) {
    return buildingHouse2x2;
  } else if(w == 2 && h == 3) {
    return buildingHouse2x3;
  } else if(w == 3 && h == 1) {
    return buildingHouse3x1;
  } else if(w == 3 && h == 2) {
    return buildingHouse3x2;
  } else if(w == 3 && h == 3) {
    return buildingHouse3x3;
  } else {
    std::cout << "Unkown house " << w << "x" << h << std::endl;
    throw "unknown house";
  }
}

void BuildingRender::renderBuilding(SDL_Surface *r, Building *b, Point where) {

  if(b->getType() == Building::TYPE_COMMERSIAL
  || b->getType() == Building::TYPE_INDUSTRIAL
  || b->getType() == Building::TYPE_RESIDENTIAL) {
  
    //std::cout << "building: " << (int)b->getWidth() << ", " << (int)b->getHeight() << std::endl;
    //std::cout << "building: " << where.getX() << ", " << where.getY() << std::endl;
    SDL_Surface *bs = getZoneBuilding(b->getWidth(), b->getHeight());
    
    SDL_Rect dst = {where.getX() - TILE_W / 2 * (b->getWidth() - 1), 
        where.getY() - bs->h + TILE_H / 2 * (b->getHeight() + b->getWidth()), bs->w, bs->h};
    SDL_BlitSurface(bs, NULL, r, &dst);
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

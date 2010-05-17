
#include "BuildingRender.hpp"

BuildingRender::BuildingRender() {

  try {

    buildingPolice      = ImageLoader::getImage("img/special/police.pcx");
    buildingFire        = ImageLoader::getImage("img/special/fire.pcx");
    buildingHospital    = ImageLoader::getImage("img/special/hospital.pcx");
    buildingPowerplant  = ImageLoader::getImage("img/special/powerplant.png");

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

    SDL_Surface *image = NULL;
    
    if(b->getType() == Building::TYPE_COMMERSIAL
            || b->getType() == Building::TYPE_INDUSTRIAL
            || b->getType() == Building::TYPE_RESIDENTIAL) {

        image = getZoneBuilding(b->getWidth(), b->getHeight());

    } else {

        switch(b->getType()) {

            case Building::TYPE_POLICE:      image = buildingPolice;     break;
            case Building::TYPE_FIRE:        image = buildingFire;       break;
            case Building::TYPE_HOSPITAL:    image = buildingHospital;   break;
            case Building::TYPE_POWERPLANT:  image = buildingPowerplant; break;
            default: throw "BuildingRender: No image loaded for building type";
        }
    }

    // Calculate position to place image correctly:
    int x = where.getX() - (b->getWidth() - 1)  * TILE_W / 2 - 1;
    int y = where.getY() - image->h + TILE_H / 2 * (b->getHeight() + b->getWidth()) + 1;

    SDL_Rect dst = {x, y, image->w, image->h};
    SDL_BlitSurface(image, NULL, r, &dst);
    
}


void BuildingRender::renderBuildingInfo(SDL_Surface *r, Building *b, Point where) {

    if(b->getType() == Building::TYPE_COMMERSIAL
            || b->getType() == Building::TYPE_INDUSTRIAL
            || b->getType() == Building::TYPE_RESIDENTIAL) {
        BuildingZone *zb = (BuildingZone *)b;
        int level = zb->getLevel();
        
        char text[255];
        sprintf(text, "%d,%i", level, b->powered);
       
        TTF_Font *font = TTF_OpenFont("DejaVuSans.ttf", 12);
        if(!font) {
            printf("TTF_OpenFont: %s\n", TTF_GetError());
            // handle error
        } else {
            SDL_Color color = {0,0,0};
            SDL_Surface *text_surface = TTF_RenderText_Solid(font, text, color);
            if (text_surface != NULL)
            {
                SDL_Rect dst2 = {where.getX() + TILE_W / 2 - text_surface->w / 2, 
                        where.getY() + TILE_H / 2 - text_surface->h / 2, 
                        text_surface->w, text_surface->h};
                SDL_BlitSurface(text_surface, NULL, r, &dst2);
                SDL_FreeSurface(text_surface);
            }
            TTF_CloseFont(font);
        }
        
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
    // XXX: Render some debug information:
    //Render depth by Z depth (TODO, render only visible)
    for(unsigned int d = 0; d < (mr->getMap()->getWidth() + mr->getMap()->getHeight()); d++) {
        for(unsigned int i = 0; i < bm->getZoneBuildingCount(); i++) {
            Building *b = bm->getZoneBuilding(i);
            if(b->getZ() == d)
                renderBuildingInfo(r, b, mr->toScreenCoord(b->getPosition(), cam));
        }
    }

}


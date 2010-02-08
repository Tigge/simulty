#include "MapRender.hpp"

//#include "../../Client.hpp"
#include "../../Tile.hpp"

MapRender::MapRender() {

  // Try to load all images:
  try {

    t_zone_res   = ImageLoader::getImage("img/terrain/terrain_zone_res.pcx");
    t_zone_com   = ImageLoader::getImage("img/terrain/terrain_zone_com.pcx");
    t_zone_ind   = ImageLoader::getImage("img/terrain/terrain_zone_ind.pcx");

    t_owned      = ImageLoader::getImage("img/terrain/terrain_owned_2.pcx");

    t_border     = new SDL_Surface *[4];
    t_border[DIR_UP]    = ImageLoader::getImage("img/line_up_terrain.pcx");
    t_border[DIR_RIGHT] = ImageLoader::getImage("img/line_right_terrain.pcx");
    t_border[DIR_DOWN]  = ImageLoader::getImage("img/line_down_terrain.pcx");
    t_border[DIR_LEFT]  = ImageLoader::getImage("img/line_left_terrain.pcx");

    mouse_hint   = ImageLoader::getImage("img/mouse_hint.pcx");

    t_terrain    = new SDL_Surface*[1];
    t_terrain[0] = ImageLoader::getImage("img/terrain/terrain_grass_1.pcx");

    t_roads      = new SDL_Surface*[16];

    // Single road tile:
    t_roads[0]   = ImageLoader::getImage("img/roads/road_straight_ns_1.pcx");

    // End tiles:
    t_roads[1]   = ImageLoader::getImage("img/roads/road_straight_ns_1.pcx");
    t_roads[2]   = ImageLoader::getImage("img/roads/road_straight_ew_1.pcx");
    t_roads[4]   = ImageLoader::getImage("img/roads/road_straight_ns_1.pcx");
    t_roads[8]   = ImageLoader::getImage("img/roads/road_straight_ew_1.pcx");

    // Straight:

    t_roads[5]   = ImageLoader::getImage("img/roads/road_straight_ns_1.pcx");
    t_roads[10]  = ImageLoader::getImage("img/roads/road_straight_ew_1.pcx");


    // Corners:
    t_roads[3]   = ImageLoader::getImage("img/roads/road_corner_ne_1.pcx");
    t_roads[6]   = ImageLoader::getImage("img/roads/road_corner_es_1.pcx");
    t_roads[12]  = ImageLoader::getImage("img/roads/road_corner_sw_1.pcx");
    t_roads[9]   = ImageLoader::getImage("img/roads/road_corner_wn_1.pcx");

    // Intersections: (3 way)

    t_roads[7]   = ImageLoader::getImage("img/roads/road_intersection_nes_1.pcx");
    t_roads[14]  = ImageLoader::getImage("img/roads/road_intersection_esw_1.pcx");
    t_roads[13]  = ImageLoader::getImage("img/roads/road_intersection_swn_1.pcx");
    t_roads[11]  = ImageLoader::getImage("img/roads/road_intersection_wne_1.pcx");

    // Intersections: (4 way)

    t_roads[15]  = ImageLoader::getImage("img/roads/road_intersection_nesw_1.pcx");

  } catch(ImageLoaderException e) {
    std::cout << "Error: " << e.what();
    exit(1);
  }
}

MapRender::~MapRender() {

  // TODO: destroy all images

  for(int i = 0; i < 4; i++)
    SDL_FreeSurface(t_border[i]);
  delete [] t_border;

  for(int i = 0; i < 1; i++)
    SDL_FreeSurface(t_terrain[i]);
  delete [] t_terrain;

  for(int i = 0; i < 16; i++)
    SDL_FreeSurface(t_roads[i]);
  delete [] t_roads;

  SDL_FreeSurface(t_owned);

  SDL_FreeSurface(t_zone_res);
  SDL_FreeSurface(t_zone_com);
  SDL_FreeSurface(t_zone_ind);

  SDL_FreeSurface(mouse_hint);

}

Map *MapRender::getMap() {
  return this->m;
}
void MapRender::setMap(Map *m) {
  this->m = m;
}


SDL_Surface *MapRender::getRoadBitmap(int x, int y)
{
  int n = (m->outOfBounds(Point(x-1, y)) ? 0 : 1 * (int)m->getTile(x - 1, y)->isRoad());
  int e = (m->outOfBounds(Point(x, y+1)) ? 0 : 2 * (int)m->getTile(x, y + 1)->isRoad());
  int s = (m->outOfBounds(Point(x+1, y)) ? 0 : 4 * (int)m->getTile(x + 1, y)->isRoad());
  int w = (m->outOfBounds(Point(x, y-1)) ? 0 : 8 * (int)m->getTile(x, y - 1)->isRoad());
  return t_roads[n + e + s + w];

}


Point MapRender::toTileCoord(Point screenCoord) {

  Point tileCoord;

  tileCoord.setY(screenCoord.getX() / TILE_W
      + screenCoord.getY() / TILE_H - (m->getHeight()) / 2);
  tileCoord.setX(screenCoord.getY() / TILE_H * 2
      - (screenCoord.getX() / TILE_W
      + screenCoord.getY() / TILE_H - (m->getHeight()) / 2));

  Point shavedScreenCoord = toScreenCoord(tileCoord);
  
  Uint32 mouse_hint_color = ImageUtils::getpixel(mouse_hint,
      screenCoord.getX() - shavedScreenCoord.getX(),
      screenCoord.getY() - shavedScreenCoord.getY());

  if(mouse_hint_color == SDL_MapRGB(mouse_hint->format, 255, 0, 0))      tileCoord.translate(0, -1);
  else if(mouse_hint_color == SDL_MapRGB(mouse_hint->format, 0, 255, 0)) tileCoord.translate(-1, 0);
  else if(mouse_hint_color == SDL_MapRGB(mouse_hint->format, 0, 0, 255)) tileCoord.translate(1,  0);
  else if(mouse_hint_color == SDL_MapRGB(mouse_hint->format, 0, 0, 0))   tileCoord.translate(0,  1);

  return tileCoord;

}

Point MapRender::toTileCoord(Point screenCoord, Camera cam) {

  screenCoord.translate(cam);
  return toTileCoord(screenCoord);

}

Point MapRender::toScreenCoord(Point tileCoord) {

  Point screenCoord;
  screenCoord.setX((m->getHeight() - (tileCoord.getX() - tileCoord.getY())) * TILE_W / 2);
  screenCoord.setY((tileCoord.getX() + tileCoord.getY()) * TILE_H / 2);
  return screenCoord;

}
Point MapRender::toScreenCoord(Point tileCoord, Camera cam) {

  Point screenCoord = toScreenCoord(tileCoord);
  screenCoord.translate(-cam.getX(), -cam.getY());
  return screenCoord;

}

void MapRender::render (SDL_Surface *b, Camera cam) {
  Point start, end;

  start = toTileCoord(cam);
  end   = toTileCoord(cam + Point(b->w, b->h));

  /*
  start = val2tile(cam);
  end   = val2tile(Point(cam.getX() + b->w, cam.getY() + b->h));
  */
  //al_trace("start %i,%i\n", start.x, start.y);
  //al_trace("end %i,%i\n", end.x, end.y);

  int base_x = start.getX() - 1;
  int base_y = start.getY() - 2;
  
  //std::cout << "Rendering from " << start << std::endl;

  //if(cam.y % TILE_H > TILE_H / 2){ base_y--; base_x--; }

  for(int i = 0; i <= b->h / (TILE_H / 2) + 6; i++) {

    //al_trace("===============================\n");
    //al_trace("base %i,%i\n", base_x, base_y);

    for(int x = base_x, y = base_y;
        y - base_y < (b->w / TILE_W) + 2; x--, y++) {
        //al_trace("p %i,%i\n", x, y);
        
        //textprintf_ex(b, font, place.x + 20 - cam.x, place.y + 10 - cam.y, makecol(255, 255, 255), -1, "%i,%i", x, y);

        // If tile is a valid one, then we draw it:
        if(y >= 0 && x >= 0 && y < (int)m->getHeight() && x < (int)m->getWidth()) {
            Point pos = toScreenCoord(Point(x, y)) - cam;
            Tile *tile = m->getTile(x, y);

            //std::cout << "drawing " << x << ", " << y << std::endl;
            //std::cout << "     to " << pos.getX() << ", " << pos.getY() << std::endl;
            // Draw terrain:
            SDL_Rect srcR = {1, 1, TILE_W, TILE_H};
            SDL_Rect dstR = {pos.getX(), pos.getY(), TILE_W, TILE_H};
            SDL_BlitSurface(t_terrain[tile->getTerrain()], &srcR, b, &dstR);
            //masked_blit(t_terrain[tile->getTerrain()], b, 1, 1, pos.getX(), pos.getY(), TILE_W, TILE_H);

            // Draw zone:
            if(tile->getZone() == SIMULTY_CLIENT_TOOL_ZONE_RES)
                SDL_BlitSurface(t_zone_res, &srcR, b, &dstR);
            else if(tile->getZone() == SIMULTY_CLIENT_TOOL_ZONE_COM)
                SDL_BlitSurface(t_zone_com, &srcR, b, &dstR);
            else if(tile->getZone() == SIMULTY_CLIENT_TOOL_ZONE_IND)
                SDL_BlitSurface(t_zone_ind, &srcR, b, &dstR);

            // Draw road:
            if(tile->isRoad())
                SDL_BlitSurface(getRoadBitmap(x, y), &srcR, b, &dstR);

            // Draw borders:
            if(tile->getOwner() != -1) {

              if(y > 0 && m->getTile(x, y - 1)->getOwner() != m->getTile(x, y)->getOwner())
                  SDL_BlitSurface(t_border[DIR_LEFT], &srcR, b, &dstR);
              if(x < (int)m->getWidth() - 1 && m->getTile(x + 1, y)->getOwner() != m->getTile(x, y)->getOwner())
                  SDL_BlitSurface(t_border[DIR_DOWN], &srcR, b, &dstR);
              if(y < (int)m->getHeight() - 1 && m->getTile(x, y + 1)->getOwner() != m->getTile(x, y)->getOwner())
                  SDL_BlitSurface(t_border[DIR_RIGHT], &srcR, b, &dstR);
              if(x > 0 && m->getTile(x - 1, y)->getOwner() != m->getTile(x, y)->getOwner())
                  SDL_BlitSurface(t_border[DIR_UP], &srcR, b, &dstR);

              //textprintf_ex(b, font, pos.getX() + 10 , pos.getY() + 16, makecol(0, 0, 0), -1, "%i,%i", tile->getOwner(), tile->getZone());
              }

            //textprintf_ex(b, font, place.x + 20 - cam.x, place.y + 10 - cam.y, makecol(0, 0, 0), -1, "%i,%i", x, y);
        }

    }

    //al_trace("bases diff %i,%i\n", base_x % 2, base_y % 2);

    if(abs(base_x % 2) == abs(base_y % 2))
        base_y++;
    else
        base_x++;

  }

}


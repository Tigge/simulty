#include "MapRender.hpp"

//#include "../../Client.hpp"
#include "../../Tile.hpp"

MapRender::MapRender() {

  // Try to load all images:
  try {

    t_zone_res   = ImageLoader::getImage("img/terrain_zone_res.pcx");
    t_zone_com   = ImageLoader::getImage("img/terrain_zone_com.pcx");
    t_zone_ind   = ImageLoader::getImage("img/terrain_zone_ind.pcx");

    t_owned      = ImageLoader::getImage("img/terrain_owned_2.pcx");

    t_border     = new BITMAP *[5];
    t_border[1]  = ImageLoader::getImage("img/line_up_terrain.pcx");
    t_border[2]  = ImageLoader::getImage("img/line_right_terrain.pcx");
    t_border[3]  = ImageLoader::getImage("img/line_down_terrain.pcx");
    t_border[4]  = ImageLoader::getImage("img/line_left_terrain.pcx");

    mouse_hint   = ImageLoader::getImage("img/mouse_hint.pcx");

    t_terrain    = new BITMAP*[1];
    t_terrain[0] = ImageLoader::getImage("img/terrain_grass_1.pcx");

    t_roads      = new BITMAP*[16];

    // Single road tile:
    t_roads[0]   = ImageLoader::getImage("img/road_straight_ns_1.pcx");

    // End tiles:
    t_roads[1]   = ImageLoader::getImage("img/road_straight_ns_1.pcx");
    t_roads[2]   = ImageLoader::getImage("img/road_straight_ew_1.pcx");
    t_roads[4]   = ImageLoader::getImage("img/road_straight_ns_1.pcx");
    t_roads[8]   = ImageLoader::getImage("img/road_straight_ew_1.pcx");

    // Straight:

    t_roads[5]   = ImageLoader::getImage("img/road_straight_ns_1.pcx");
    t_roads[10]  = ImageLoader::getImage("img/road_straight_ew_1.pcx");


    // Corners:
    t_roads[3]   = ImageLoader::getImage("img/road_corner_ne_1.pcx");
    t_roads[6]   = ImageLoader::getImage("img/road_corner_es_1.pcx");
    t_roads[12]  = ImageLoader::getImage("img/road_corner_sw_1.pcx");
    t_roads[9]   = ImageLoader::getImage("img/road_corner_wn_1.pcx");

    // Intersections: (3 way)

    t_roads[7]   = ImageLoader::getImage("img/road_intersection_nes_1.pcx");
    t_roads[14]  = ImageLoader::getImage("img/road_intersection_esw_1.pcx");
    t_roads[13]  = ImageLoader::getImage("img/road_intersection_swn_1.pcx");
    t_roads[11]  = ImageLoader::getImage("img/road_intersection_wne_1.pcx");

    // Intersections: (4 way)

    t_roads[15]  = ImageLoader::getImage("img/road_intersection_nesw_1.pcx");

  } catch(ImageLoaderException e) {
    allegro_message("Error: %s", e.what());
    exit(1);
  }
}

MapRender::~MapRender() {

  // TODO: destroy all images

  for(int i = 1; i < 5; i++)
    destroy_bitmap(t_border[i]);
  delete [] t_border;


  for(int i = 0; i < 1; i++)
    destroy_bitmap(t_terrain[i]);
  delete [] t_terrain;

  for(int i = 0; i < 16; i++)
    destroy_bitmap(t_roads[i]);
  delete [] t_roads;
  
  destroy_bitmap(t_owned);
    
  destroy_bitmap(t_zone_res);
  destroy_bitmap(t_zone_com);
  destroy_bitmap(t_zone_ind);
    
  destroy_bitmap(mouse_hint);

}

Map *MapRender::getMap() {
  return this->m;
}
void MapRender::setMap(Map *m) {
  this->m = m;
}


BITMAP *MapRender::getRoadBitmap(int x, int y)
{
  int n = 1 * (int)m->getTile(x - 1, y)->isRoad();
  int e = 2 * (int)m->getTile(x, y + 1)->isRoad();
  int s = 4 * (int)m->getTile(x + 1, y)->isRoad();
  int w = 8 * (int)m->getTile(x, y - 1)->isRoad();
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

  int mouse_hint_color = getpixel(mouse_hint,
      screenCoord.getX() - shavedScreenCoord.getX(),
      screenCoord.getY() - shavedScreenCoord.getY());

  if(mouse_hint_color == makecol(255, 0, 0))      tileCoord.translate(0, -1);
  else if(mouse_hint_color == makecol(0, 255, 0)) tileCoord.translate(-1, 0);
  else if(mouse_hint_color == makecol(0, 0, 255)) tileCoord.translate(1,  0);
  else if(mouse_hint_color == makecol(0, 0, 0))   tileCoord.translate(0,  1);

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

void MapRender::render (BITMAP *b, Camera cam) {
  Point start, end;

  start = toTileCoord(cam);
  end   = toTileCoord(cam + Point(SCREEN_W, SCREEN_H));

  /*
  start = val2tile(cam);
  end   = val2tile(Point(cam.getX() + SCREEN_W, cam.getY() + SCREEN_H));
  */
  //al_trace("start %i,%i\n", start.x, start.y);
  //al_trace("end %i,%i\n", end.x, end.y);

  int base_x = start.getX() - 1;
  int base_y = start.getY() - 2;

  //if(cam.y % TILE_H > TILE_H / 2){ base_y--; base_x--; }

  for(int i = 0; i <= SCREEN_H / TILE_H * 2 + 6; i++) {

    //al_trace("===============================\n");
    //al_trace("base %i,%i\n", base_x, base_y);

    for(unsigned int x = base_x, y = base_y; 
        x >= (unsigned int)base_x - 2 - SCREEN_W / TILE_W  
        && y != (unsigned int)base_y + SCREEN_W; 
        x--, y++) {
        //al_trace("p %i,%i\n", x, y);


        //textprintf_ex(b, font, place.x + 20 - cam.x, place.y + 10 - cam.y, makecol(255, 255, 255), -1, "%i,%i", x, y);

        // If tile is a valid one, then we draw it:
        if(y >= 0 && x >= 0 && y < m->getHeight() && x < m->getWidth())
        {
            Point pos = toScreenCoord(Point(x, y)) - cam;
            Tile *tile = m->getTile(x, y);

            // Draw terrain:
            masked_blit(t_terrain[tile->getTerrain()], b, 1, 1, pos.getX(), pos.getY(), TILE_W, TILE_H);

            // Draw zone:
            if(tile->getZone() == SIMULTY_CLIENT_TOOL_ZONE_RES)
                masked_blit(t_zone_res, b, 1, 1, pos.getX(), pos.getY(), TILE_W, TILE_H);
            else if(tile->getZone() == SIMULTY_CLIENT_TOOL_ZONE_COM)
                masked_blit(t_zone_com, b, 1, 1, pos.getX(), pos.getY(), TILE_W, TILE_H);
            else if(tile->getZone() == SIMULTY_CLIENT_TOOL_ZONE_IND)
                masked_blit(t_zone_ind, b, 1, 1, pos.getX(), pos.getY(), TILE_W, TILE_H);

            // Draw road:
            if(tile->isRoad())
                masked_blit(getRoadBitmap(x, y), b, 1, 1, pos.getX(), pos.getY(), TILE_W, TILE_H);

            // Draw borders:
            if(tile->getOwner() != -1) {

              if(y > 0 && m->getTile(x, y - 1)->getOwner() != m->getTile(x, y)->getOwner())
                  masked_blit(t_border[DIR_LEFT], b, 1, 1, pos.getX(), pos.getY(), TILE_W, TILE_H);
              if(x < m->getWidth() - 1 && m->getTile(x + 1, y)->getOwner() != m->getTile(x, y)->getOwner())
                  masked_blit(t_border[DIR_DOWN], b, 1, 1, pos.getX(), pos.getY(), TILE_W, TILE_H);
              if(y < m->getHeight() - 1 && m->getTile(x, y + 1)->getOwner() != m->getTile(x, y)->getOwner())
                  masked_blit(t_border[DIR_RIGHT], b, 1, 1, pos.getX(), pos.getY(), TILE_W, TILE_H);
              if(x > 0 && m->getTile(x - 1, y)->getOwner() != m->getTile(x, y)->getOwner())
                  masked_blit(t_border[DIR_UP], b, 1, 1, pos.getX(), pos.getY(), TILE_W, TILE_H);

              //textprintf_ex(b, font, place.x + 20 - cam.x, place.y + 10 - cam.y, makecol(0, 0, 0), -1, "O");
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


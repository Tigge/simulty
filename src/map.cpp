
#include "client.h"

#include "map.h"
#include "tile.h"

map::map() {
    t_zone_res     = load_bitmap("img/terrain_zone_res.pcx",  NULL);
    t_zone_com     = load_bitmap("img/terrain_zone_com.pcx",  NULL);
    t_zone_ind     = load_bitmap("img/terrain_zone_ind.pcx",  NULL);
    clean_bitmap(t_zone_res); clean_bitmap(t_zone_com); clean_bitmap(t_zone_ind);


    t_owned     = load_bitmap("img/terrain_owned_2.pcx",  NULL);
    clean_bitmap(t_owned);
    
    t_border     = new BITMAP*[5];
    t_border[1]  = load_bitmap("img/line_up_terrain.pcx", NULL);
    t_border[2]  = load_bitmap("img/line_right_terrain.pcx", NULL);
    t_border[3]  = load_bitmap("img/line_down_terrain.pcx", NULL);
    t_border[4]  = load_bitmap("img/line_left_terrain.pcx", NULL);
    
    for(int i = 1; i < 5; i++)
    {
        clean_bitmap(t_border[i]);
    }

    mouse_hint  = load_bitmap("img/mouse_hint.pcx",  NULL);

    t_terrain    = new BITMAP*[1];
    t_terrain[0] = load_bitmap("img/terrain_grass_1.pcx", NULL);
        
    clean_bitmap(t_terrain[0]);    
    
    t_roads      = new BITMAP*[16];

    // Single road tile:
    t_roads[0]   = load_bitmap("img/road_straight_ns_1.pcx", NULL);

    // End tiles:
    t_roads[1]  = load_bitmap("img/road_straight_ns_1.pcx", NULL);
    t_roads[2]  = load_bitmap("img/road_straight_ew_1.pcx", NULL);
    t_roads[4]  = load_bitmap("img/road_straight_ns_1.pcx", NULL);
    t_roads[8]  = load_bitmap("img/road_straight_ew_1.pcx", NULL);

    // Straight:

    t_roads[5]  = load_bitmap("img/road_straight_ns_1.pcx", NULL);
    t_roads[10] = load_bitmap("img/road_straight_ew_1.pcx", NULL);


    // Corners:
    t_roads[3]  = load_bitmap("img/road_corner_ne_1.pcx", NULL);
    t_roads[6]  = load_bitmap("img/road_corner_es_1.pcx", NULL);
    t_roads[12] = load_bitmap("img/road_corner_sw_1.pcx", NULL);
    t_roads[9]  = load_bitmap("img/road_corner_wn_1.pcx", NULL);
    
    // Intersections: (3 way)

    t_roads[7]  = load_bitmap("img/road_intersection_nes_1.pcx", NULL);
    t_roads[14] = load_bitmap("img/road_intersection_esw_1.pcx", NULL);
    t_roads[13] = load_bitmap("img/road_intersection_swn_1.pcx", NULL);
    t_roads[11] = load_bitmap("img/road_intersection_wne_1.pcx", NULL);

    // Intersections: (4 way)

    t_roads[15]   = load_bitmap("img/road_intersection_nesw_1.pcx", NULL);


    for(int i = 0; i < 16; i++)
    {
        if(!t_roads[i])allegro_message("Failed to load road #%i", i);
        clean_bitmap(t_roads[i]);
    }

}

map::~map()
{

for(int i = 0; i < 1; i++)
    destroy_bitmap(t_terrain[i]);

delete [] t_terrain;

for(int i = 0; i < 3; i++)
    destroy_bitmap(t_roads[i]);

delete [] t_roads;

}



BITMAP *map::road_tile(int x, int y)
{

    int n = 1 * (int)tiles[y * width + (x - 1)].road;
    int e = 2 * (int)tiles[(y + 1) * width + x].road;
    int s = 4 * (int)tiles[y * width + (x + 1)].road;
    int w = 8 * (int)tiles[(y - 1) * width + x].road;

    return t_roads[n + e + s + w];

}



void map::clean_bitmap(BITMAP *b)
{

    if(b)
    {
        int green = makecol(0, 255, 0);
        int pink  = makecol(255, 0, 255);

        for(int x = 0; x < b->w; x++)
            for(int y = 0; y < b->h; y++)
            {
                if(((short *)b->line[y])[x] == green)((short *)b->line[y])[x] = pink;
            }
    }
   
}


Point map::val2tile(Point input)
{
    Point output;    

    output.setY(input.getX() / TILE_W + input.getY() / TILE_H - (height) / 2);
    output.setX(input.getY() / TILE_H * 2 - (input.getX() / TILE_W + input.getY() / TILE_H - (height) / 2));

    return output;
}

Point map::val2tile_real(Point input)
{
    Point base = val2tile(input);
    Point pos  = tile2val(base);

    int mouse_hint_color = getpixel(mouse_hint, input.getX() - pos.getX(), input.getY() - pos.getY());

    if(mouse_hint_color == makecol(255, 0, 0))      base.translate(0, -1);
    else if(mouse_hint_color == makecol(0, 255, 0)) base.translate(-1, 0);
    else if(mouse_hint_color == makecol(0, 0, 255)) base.translate(1,  0);
    else if(mouse_hint_color == makecol(0, 0, 0))   base.translate(0,  1);             

    return base;
}


Point map::tile2val(Point input)
{
    Point output;

    output.setX((height - (input.getX() - input.getY())) * TILE_W / 2);
    output.setY((input.getX() + input.getY()) * TILE_H / 2);

    return output;
}



void map::render (BITMAP *b, Point cam)
{
    Point start, end;

    start = val2tile(cam);
    end   = val2tile(Point(cam.getX() + SCREEN_W, cam.getY() + SCREEN_H));

    //al_trace("start %i,%i\n", start.x, start.y);
    //al_trace("end %i,%i\n", end.x, end.y);

    int base_x = start.getX() - 1;
    int base_y = start.getY() - 2;

    //if(cam.y % TILE_H > TILE_H / 2){ base_y--; base_x--; }

    for(int i = 0; i <= SCREEN_H / TILE_H * 2 + 6; i++)
    {

        //al_trace("===============================\n");
        //al_trace("base %i,%i\n", base_x, base_y);

        for(int x = base_x, y = base_y; x >= base_x - 2 - SCREEN_W / TILE_W  && y != base_y + SCREEN_W; x--, y++)
        {
            //al_trace("p %i,%i\n", x, y);

            
            Point place = tile2val(Point(x, y));

            //textprintf_ex(b, font, place.x + 20 - cam.x, place.y + 10 - cam.y, makecol(255, 255, 255), -1, "%i,%i", x, y);

            // If tile is a valid one, then we draw it:
            if(y >= 0 && x >= 0 && y < height && x < width)
            {
                // Draw terrain:            
                masked_blit(t_terrain[tiles[(y) * width + (x)].terrain], b, 1, 1, place.getX() - cam.getX(), place.getY() - cam.getY(), TILE_W, TILE_H);
               
                // Draw zone:
                if(tiles[y * width + x].zone == SIMULTY_CLIENT_TOOL_ZONE_RES)
                    masked_blit(t_zone_res, b, 1, 1, place.getX() - cam.getX(), place.getY() - cam.getY(), TILE_W, TILE_H);
                else if(tiles[y * width + x].zone == SIMULTY_CLIENT_TOOL_ZONE_COM)
                    masked_blit(t_zone_com, b, 1, 1, place.getX() - cam.getX(), place.getY() - cam.getY(), TILE_W, TILE_H);
                else if(tiles[y * width + x].zone == SIMULTY_CLIENT_TOOL_ZONE_IND)
                    masked_blit(t_zone_ind, b, 1, 1, place.getX() - cam.getX(), place.getY() - cam.getY(), TILE_W, TILE_H);

                // Draw road:
                if(tiles[y * width + x].road)
                    masked_blit(road_tile(x, y), b, 1, 1, place.getX() - cam.getX(), place.getY() - cam.getY(), TILE_W, TILE_H);

                // Draw info (debug)
                if(tiles[y * width + x].owner != -1)
                {                
                  if(tiles[(y - 1) * width + x].owner != tiles[y * width + x].owner )
                      masked_blit(t_border[DIR_LEFT], b, 1, 1, place.getX() - cam.getX(), place.getY() - cam.getY(), TILE_W, TILE_H);                
                  if(tiles[y * width + (x + 1)].owner != tiles[y * width + x].owner )
                      masked_blit(t_border[DIR_DOWN], b, 1, 1, place.getX() - cam.getX(), place.getY() - cam.getY(), TILE_W, TILE_H);                
                  if(tiles[(y + 1) * width + x].owner != tiles[y * width + x].owner )
                      masked_blit(t_border[DIR_RIGHT], b, 1, 1, place.getX() - cam.getX(), place.getY() - cam.getY(), TILE_W, TILE_H);                
                  if(tiles[y * width + x - 1].owner != tiles[y * width + x].owner )
                      masked_blit(t_border[DIR_UP], b, 1, 1, place.getX() - cam.getX(), place.getY() - cam.getY(), TILE_W, TILE_H);   
                                                                                               
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


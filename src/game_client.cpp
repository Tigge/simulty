#include "allegro.h"

#include "shared.h"

#include "game_client.h"

#include "map_base.h"
#include "map.h"

#include "player.h"
#include "player_client_local.h"
#include "player_client_remote.h"

#define err cerr


// TODO - static functions instead?
void speedhandler(void *data)
{
    ((game_client *)data)->speed_counter++;
}
END_OF_FUNCTION()

void fpshandler(void *data)
{
    ((game_client *)data)->fps = ((game_client *)data)->frames;
    ((game_client *)data)->frames = 0;
}
END_OF_FUNCTION()

void mousehandler(int flags)
{

}


game_client::game_client()
{

    // Initializing allegro (and some sub elements)
    allegro_init();

    install_keyboard();
    install_timer();
    install_mouse();

    // Locking variables and functions
    LOCK_VARIABLE(fps);
    LOCK_VARIABLE(speed_counter);
    LOCK_VARIABLE(frames);

    LOCK_FUNCTION(speedhandler);
    LOCK_FUNCTION(fpshandler);

    // Setting color depth
    cout << "Allegro inited..." << endl;

    set_color_depth(16);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
    set_color_conversion(COLORCONV_TOTAL | COLORCONV_KEEP_TRANS);

    // Create double buffer
    buffer = create_bitmap(800, 600);    
    
    mouse_pointer = load_bitmap("img/cursor.pcx", NULL);
    mouse_block  = load_bitmap("img/mouse_block.pcx", NULL);

    if(!mouse_pointer || !mouse_block || !buffer)
    {
        allegro_message("Couldn't load / create some images");
        exit(1);
    }

    // Add local socket and connect it to server (TODO: move later)
    net_client = net.add();
    net_client->connect_to("home.tigge.org", 5557);

    install_param_int_ex(speedhandler, this, BPS_TO_TIMER(60));
    install_param_int_ex(fpshandler, this, BPS_TO_TIMER(1));

    fps =  speed_counter =  frames = 0;

    state_running = true;
    state_menu  = true;
    state_game = false;

    m = new map;
    g = new gui;

    time = 0;

    // Init mouse handler:

    mouse = new mouse_handler;

    // Set max players and set up all slots:

    player_me = NULL;

}

game_client::~game_client (){


    // Delete mouse handler:
    delete mouse;

}

bool game_client::needupdate()
{
    return speed_counter > 0;
}

bool game_client::running()
{
    return state_running;
}

void game_client::render ()
{
    // Clear the double buffer:
    clear_bitmap(buffer);

    if(state_game)
    {
        
        if(state_game == SIMULTY_CLIENT_STATE_GAME_ON)
        {

            m->render(buffer, cam);

            point pos = m->val2tile(point(mouse_x + cam.x, mouse_y + cam.y));            
            point ral = m->val2tile_real(point(mouse_x + cam.x, mouse_y + cam.y));


            point xpos = m->tile2val(pos);
            point xral = m->tile2val(ral);
          

            if(mouse->button_left == BUTTON_IS_DOWN)
            {
                //point ms = m->val2tile_real();

                point c1 = mouse_down_tile;
                point c3 = m->val2tile_real(point(mouse_x + cam.x, mouse_y + cam.y));

                point c2 = point(c3.x, c1.y);
                point c4 = point(c1.x, c3.y);

                c1 = m->tile2val(c1); c2 = m->tile2val(c2); c3 = m->tile2val(c3); c4 = m->tile2val(c4);

                point mt = m->val2tile_real(point(mouse_x + cam.x, mouse_y + cam.y));

                int points[8] = { c1.x - cam.x + TILE_W / 2, c1.y - cam.y + TILE_H / 2,
                                    c2.x - cam.x + TILE_W / 2, c2.y - cam.y + TILE_H / 2,
                                    c3.x - cam.x + TILE_W / 2, c3.y - cam.y + TILE_H / 2,
                                    c4.x - cam.x + TILE_W / 2, c4.y - cam.y + TILE_H / 2, };

                polygon(buffer, 4, points, makecol(255, 255, 255));

                point pos = m->tile2val(m->val2tile_real(point(mouse_x + cam.x, mouse_y + cam.y)));

                masked_blit(mouse_block, buffer, 0, 0, pos.x - cam.x, pos.y - cam.y, mouse_block->w, mouse_block->h);                

                textprintf_ex(buffer, font, SCREEN_W - 200, SCREEN_H - 260, makecol(0, 0, 0), -1, "%i, %i", c3.x, c3.y);
        

                //rect(buffer, mouse_down_tile_x * TILE_W, , mouse_to_tile_x() * TILE_W, mouse_to_tile_y() * TILE_H / 2, makecol(255, 0, 0));
                
            }
            //blit(mouse_hint, buffer, 0, 0, (mouse_x / TILE_W) * TILE_W, (mouse_y / TILE_H) * TILE_H, mouse_hint->w, mouse_hint->h);

        }        


    }

    // Render gui:
    g->render(buffer);

    masked_blit(mouse_pointer, buffer, 0, 0, mouse_x, mouse_y, 32, 32);

    // Render buffer to screen:
    blit(buffer, screen, 0, 0, 0, 0, 800, 600);

    // Increase number of frames:
    frames++;
}

bool game_client::cam_move_step(int dir, int step = 1)
{

         if(dir == DIR_UP    && cam.y - step >= 0       ) { cam.y -= step; return true; }
    else if(dir == DIR_RIGHT && cam.x + step < m->width * TILE_W ) { cam.x += step; return true; }
    else if(dir == DIR_DOWN  && cam.y + step < m->height * TILE_H ) { cam.y += step; return true; }
    else if(dir == DIR_LEFT  && cam.x - step >= 0       ) { cam.x -= step; return true; }

    return false;
}

bool game_client::cam_move_jump(int x, int y)
{

    return true;
}

void game_client::update (  )
{

    speed_counter--;  

    mouse->update();

    // Update gui
    g->update();


    //cout << ",";



    if(state_menu)
    {
        if(mouse_b & 1) { state_menu = false; state_game = SIMULTY_CLIENT_STATE_GAME_ON; }

    }
    else if(state_game)
    {


        if(mouse->button_left == BUTTON_DOWN)
        {
            mouse_down_tile = m->val2tile_real(point(mouse_x + cam.x, mouse_y + cam.y));
        }

        if(mouse->button_left == BUTTON_UP)
        {
            point mouse_up_tile = m->val2tile_real(point(mouse_x + cam.x, mouse_y + cam.y));

            if(tool == SIMULTY_CLIENT_TOOL_LAND)
            {
                NPacket landpak(NPACKET_TYPE_SIMULTY_LAND_BUY);
                landpak << (INT32)mouse_down_tile.x << (INT32) mouse_down_tile.y 
                        << (INT32)mouse_up_tile.x   << (INT32) mouse_up_tile.y;
                net_client->packet_put(landpak);

                // buy land
            }
            else if(tool == SIMULTY_CLIENT_TOOL_ROAD)
            {
                //point mt = m->pos_to_tile(point(mouse_x, mouse_y), cam);

                point mouse_up_tile = m->val2tile_real(point(mouse_x + cam.x, mouse_y + cam.y));

                if(!m->tiles[mouse_up_tile.y * m->width + mouse_up_tile.x].road)
                {
                    NPacket roadpak(NPACKET_TYPE_SIMULTY_ROAD_BUILD);
                    roadpak << (INT32)mouse_up_tile.x << (INT32)mouse_up_tile.y;
                    net_client->packet_put(roadpak);
                }

            }
            else if(tool == SIMULTY_CLIENT_TOOL_ZONE_RES || 
                    tool == SIMULTY_CLIENT_TOOL_ZONE_COM || 
                    tool == SIMULTY_CLIENT_TOOL_ZONE_IND)
            {
                NPacket zonepak(NPACKET_TYPE_SIMULTY_LAND_ZONE);
                
                zonepak << (INT16)tool
                        << (INT32)mouse_down_tile.x << (INT32) mouse_down_tile.y 
                        << (INT32)mouse_up_tile.x   << (INT32) mouse_up_tile.y;
                net_client->packet_put(zonepak);     
                
                
                std:cout << (INT16)tool
                         << (INT32)mouse_down_tile.x << (INT32) mouse_down_tile.y 
                         << (INT32)mouse_up_tile.x   << (INT32) mouse_up_tile.y;         
            }
            



            //textprintf_ex(buffer, font, mh_x, mh_y, makecol(0, 0, 0), -1, "%i, %i", mouse_to_tile_x(), mouse_to_tile_y());

        }





        if(key[KEY_UP]    || mouse_y < 15           )cam_move_step(DIR_UP,    3);        
        if(key[KEY_RIGHT] || mouse_x > SCREEN_W - 15)cam_move_step(DIR_RIGHT, 3);
        if(key[KEY_DOWN]  || mouse_y > SCREEN_H - 15)cam_move_step(DIR_DOWN,  3);
        if(key[KEY_LEFT]  || mouse_x < 15           )cam_move_step(DIR_LEFT,  3);



        if(mouse_b & 1)
        {


        }


        net.update(0);

        if(net_client->packet_exists())
        {
            //err << "* Client have recieved a package... " << endl;
            packet_handle(net_client->packet_get());
            

        }        
     }
}


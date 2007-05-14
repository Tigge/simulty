#include "gui.h"
#include "client.h"
#include "allegro.h"


gui::gui (  ){


    menu_background = load_bitmap("img/menubg.pcx", NULL);

    mouse_pointer = load_bitmap("img/cursor.pcx", NULL);
    mouse_block  = load_bitmap("img/mouse_block.pcx", NULL);

    console_show = false;

    // Init mouse handler:
    mouse = new mouse_handler;
    
    if(!mouse_pointer || !mouse_block || ! menu_background)
    {
        allegro_message("Couldn't load / create some images");
        exit(1);
    }    


    tool = 0;
}

gui::~gui (  ){


    // Delete mouse handler:
    delete mouse;
}

void gui::render ( BITMAP *buffer ){

    if(client->state_menu)
    {
        blit(menu_background, buffer, 0, 0, 0, 0, 800, 600);

        textprintf_ex(buffer, font, SCREEN_W - 200, SCREEN_H - 300, makecol(0, 0, 0), -1, "New local game");
        textprintf_ex(buffer, font, SCREEN_W - 200, SCREEN_H - 280, makecol(0, 0, 0), -1, "New network game");
        textprintf_ex(buffer, font, SCREEN_W - 200, SCREEN_H - 260, makecol(0, 0, 0), -1, "Join network game");

        textprintf_ex(buffer, font, SCREEN_W - 200, SCREEN_H - 200, makecol(0, 0, 0), -1, "Quit");


    }
    else if(client->state_game)
    {
    
    
        point pos = client->m->val2tile(point(mouse_x + client->cam.x, mouse_y + client->cam.y));            
        point ral = client->m->val2tile_real(point(mouse_x + client->cam.x, mouse_y + client->cam.y));


        point xpos = client->m->tile2val(pos);
        point xral = client->m->tile2val(ral);
      
        if(mouse->button_left == EVENT_PRESS) {
        
            mouse_down_tile = mouse->down;
            mouse_down_tile.translate(client->cam);
            mouse_down_tile = client->m->val2tile_real(mouse_down_tile);
        
        } if(mouse->button_left == EVENT_HOLD) {
        
            mouse_up_tile = mouse->pos;
            mouse_up_tile.translate(client->cam);
            mouse_up_tile = client->m->val2tile_real(mouse_up_tile);
               
            point cam = client->cam;
            
            //point ms = m->val2tile_real();
            point c1 = mouse_down_tile;
            point c3 = mouse_up_tile;
            
            point::fix_points(c1, c3);

            point c2 = point(c3.x, c1.y);
            point c4 = point(c1.x, c3.y);

            c1 = client->m->tile2val(c1); c2 = client->m->tile2val(c2); 
            c3 = client->m->tile2val(c3); c4 = client->m->tile2val(c4);

            //point mt = client->m->val2tile_real(point(mouse_x + client->cam.x, client->mouse_y + cam.y));

            int points[8] = { c1.x - cam.x + TILE_W / 2, c1.y - cam.y + TILE_H / 2,
                              c2.x - cam.x + TILE_W / 2, c2.y - cam.y + TILE_H / 2,
                              c3.x - cam.x + TILE_W / 2, c3.y - cam.y + TILE_H / 2,
                              c4.x - cam.x + TILE_W / 2, c4.y - cam.y + TILE_H / 2, };

            polygon(buffer, 4, points, makecol(255, 255, 255));

            //point pos = client->m->tile2val(client->m->val2tile_real(point(mouse_x + cam.x, mouse_y + cam.y)));

            masked_blit(mouse_block, buffer, 0, 0, pos.x - cam.x, pos.y - cam.y, mouse_block->w, mouse_block->h);                

            textprintf_ex(buffer, font, SCREEN_W - 200, SCREEN_H - 260, makecol(0, 0, 0), -1, "%i, %i", c3.x, c3.y);


            //rect(buffer, mouse_down_tile_x * TILE_W, , mouse_to_tile_x() * TILE_W, mouse_to_tile_y() * TILE_H / 2, makecol(255, 0, 0));
            
        }
        //blit(mouse_hint, buffer, 0, 0, (mouse_x / TILE_W) * TILE_W, (mouse_y / TILE_H) * TILE_H, mouse_hint->w, mouse_hint->h);    

        rectfill(buffer, 0, SCREEN_H - 50, SCREEN_W, SCREEN_H, makecol(200, 200, 200));

        textprintf_ex(buffer, font, 20, SCREEN_H - 40, makecol(0, 0, 0), -1, "Money: %i", client->money); 
        textprintf_ex(buffer, font, 20, SCREEN_H - 30, makecol(0, 0, 0), -1, "Time: %i", client->time); 
        textprintf_ex(buffer, font, 20, SCREEN_H - 20, makecol(0, 0, 0), -1, "Tool: %i", tool); 

        textprintf_ex(buffer, font, 200, SCREEN_H - 20, makecol(0, 0, 0), -1, "FPS: %i", client->fps); 

        point cam = client->cam;
        point realpos  = point(mouse->pos.x + cam.x, mouse->pos.y + cam.y);
        point realtile = client->m->val2tile_real(realpos);


        textprintf_ex(buffer, font, 200, SCREEN_H - 30, makecol(0, 0, 0), -1, "Mouse: %i, %i", realtile.x, realtile.y); 

        if(realtile.x > 5 && realtile.y > 5)
        textprintf_ex(buffer, font, 300, SCREEN_H - 30, makecol(0, 0, 0), -1, "Thrive: %i", client->bman.thrive_value_get(client->m, client->player_me->slot_get(), realtile.x, realtile.y));


        if(console_show)
        {
            rectfill(buffer, 0, 0, SCREEN_W, 100, makecol(50, 50, 50)); 
            
            for(int i = 1; i <= 5; i++)
            {
                if(console_data.size() - i >= 0 && console_data.size() - i < console_data.size())
                    textprintf_ex(buffer, font, 10, 15*i, makecol(255, 255, 255), -1, "> %s", console_data[console_data.size() - i].c_str()); 
            }
        }

    }    
    
    masked_blit(mouse_pointer, buffer, 0, 0, mouse->pos.x, mouse->pos.y, 32, 32);

}


void gui::update()
{

    mouse->update();
    

    

    if(client->state_menu)
    {



    }
    else if(client->state_game)
    {
        // Mouse input:
        if(mouse->button_left == EVENT_PRESS) {
            std::cout << "mouse press event" << std::endl;
            mouse_down_tile = client->m->val2tile_real(point(mouse->pos.x + client->cam.x, mouse->pos.y + client->cam.y));
        }
        else if(mouse->button_left == EVENT_RELEASE) {    
            std::cout << "mouse release event" << std::endl;
            mouse_up_tile   = client->m->val2tile_real(point(mouse->pos.x + client->cam.x, mouse->pos.y + client->cam.y));
             
            point::fix_points(mouse_down_tile, mouse_up_tile);
            
            if(tool == SIMULTY_CLIENT_TOOL_LAND) {
                client->buy_land(mouse_down_tile, mouse_up_tile);
                // buy land
            } else if(tool == SIMULTY_CLIENT_TOOL_ROAD) {
                // draw road
                client->buy_road(mouse_down_tile, mouse_up_tile);
            } else if(tool == SIMULTY_CLIENT_TOOL_ZONE_RES || 
                    tool == SIMULTY_CLIENT_TOOL_ZONE_COM || 
                    tool == SIMULTY_CLIENT_TOOL_ZONE_IND) {
                // zone 
                client->buy_zone(mouse_down_tile, mouse_up_tile, tool);
            }
            
                
        }
    
        if(keypressed()) {
            if(key[KEY_ESC])client->state_running = false;
            if(key[KEY_F1])console_show = !console_show;

            if(key[KEY_PLUS_PAD])tool++;
            if(key[KEY_MINUS_PAD])tool--;
            
              

            if(key[KEY_UP]    || mouse_y < 15           )client->cam_move_step(DIR_UP,    3);        
            if(key[KEY_RIGHT] || mouse_x > SCREEN_W - 15)client->cam_move_step(DIR_RIGHT, 3);
            if(key[KEY_DOWN]  || mouse_y > SCREEN_H - 15)client->cam_move_step(DIR_DOWN,  3);
            if(key[KEY_LEFT]  || mouse_x < 15           )client->cam_move_step(DIR_LEFT,  3);
            
            clear_keybuf();
        }

    }    



}


void gui::console_log(std::string s)
{
    console_data.push_back(s);
}

#include "GUI.hpp"
#include "Client.hpp"


GUI::GUI (  ){


    menu_background = load_bitmap("img/menubg.pcx", NULL);
    gui_background  = load_bitmap("img/guibg.pcx", NULL);

    mouse_pointer   = load_bitmap("img/cursor.pcx", NULL);
    mouse_block     = load_bitmap("img/mouse_block.pcx", NULL);
    
    icon_ind        = load_bitmap("img/menu_icon_ind.pcx", NULL);
    icon_res        = load_bitmap("img/menu_icon_res.pcx", NULL);
    icon_com        = load_bitmap("img/menu_icon_com.pcx", NULL);        

    console_show = false;
    
    if(!mouse_pointer || !mouse_block || !menu_background || !gui_background)
    {
        allegro_message("Couldn't load / create some images");
        exit(1);
    }    


    tool = 0;
}

GUI::~GUI (  ){

}

void GUI::render ( BITMAP *buffer ){

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
    
        client->map->render(buffer, camera);
      
    
        Point pos = client->map->val2tile(Point(mouse_x + client->cam.getX(), mouse_y + client->cam.getY()));            
        Point ral = client->map->val2tile_real(Point(mouse_x + client->cam.getX(), mouse_y + client->cam.getY()));


        Point xpos = client->map->tile2val(pos);
        Point xral = client->map->tile2val(ral);
      
        if(mouse.getLeftButtonState() == STATE_PRESS) {
        

        
        } if(mouse.getLeftButtonState() == STATE_HOLD) {
        
            Point cam = client->cam;
            
            //Point ms = m->val2tile_real();
            Point c1 = mouse_down_tile;
            Point c3 = mouse_up_tile;
            
            Point::fix_points(c1, c3);

            Point c2 = Point(c3.getX(), c1.getY());
            Point c4 = Point(c1.getX(), c3.getY());

            c1 = client->map->tile2val(c1); c2 = client->map->tile2val(c2); 
            c3 = client->map->tile2val(c3); c4 = client->map->tile2val(c4);

            //Point mt = client->map->val2tile_real(Point(mouse_x + client->cam.x, client->mapouse_y + cam.y));

            int points[8] = { c1.getX() - cam.getX() + TILE_W / 2, c1.getY() - cam.getY() + TILE_H / 2,
                              c2.getX() - cam.getX() + TILE_W / 2, c2.getY() - cam.getY() + TILE_H / 2,
                              c3.getX() - cam.getX() + TILE_W / 2, c3.getY() - cam.getY() + TILE_H / 2,
                              c4.getX() - cam.getX() + TILE_W / 2, c4.getY() - cam.getY() + TILE_H / 2, };

            polygon(buffer, 4, points, makecol(255, 255, 255));

            //Point pos = client->map->tile2val(client->map->val2tile_real(Point(mouse_x + cam.x, mouse_y + cam.y)));

            masked_blit(mouse_block, buffer, 0, 0, pos.getX() - cam.getX(), pos.getY() - cam.getY(), mouse_block->w, mouse_block->h);

            textprintf_ex(buffer, font, SCREEN_W - 200, SCREEN_H - 260, makecol(0, 0, 0), -1, "%i, %i", c3.getX(), c3.getY());


            //rect(buffer, mouse_down_tile_x * TILE_W, , mouse_to_tile_x() * TILE_W, mouse_to_tile_y() * TILE_H / 2, makecol(255, 0, 0));
            
        }
        //blit(mouse_hint, buffer, 0, 0, (mouse_x / TILE_W) * TILE_W, (mouse_y / TILE_H) * TILE_H, mouse_hint->w, mouse_hint->h);    
        
        masked_blit(gui_background, buffer, 0, 0, 0, 0, gui_background->w, gui_background->h);
        
        blit(icon_com, buffer, 0, 0, SCREEN_W - 37, 5,  32, 32);
        blit(icon_res, buffer, 0, 0, SCREEN_W - 74, 5,  32, 32);
        blit(icon_ind, buffer, 0, 0, SCREEN_W - 37, 42, 32, 32);

        textprintf_ex(buffer, font, 20, SCREEN_H - 40, makecol(0, 0, 0), -1, "Money: %i", client->money); 
        textprintf_ex(buffer, font, 20, SCREEN_H - 30, makecol(0, 0, 0), -1, "Time: %i", client->time); 
        textprintf_ex(buffer, font, 20, SCREEN_H - 20, makecol(0, 0, 0), -1, "Tool: %i", tool); 

        textprintf_ex(buffer, font, 200, SCREEN_H - 20, makecol(0, 0, 0), -1, "FPS: %i", client->fps); 

        Point cam = client->cam;
        Point realpos  = Point(mouse.getPosition().getX() + cam.getX(), mouse.getPosition().getY() + cam.getY());
        Point realtile = client->map->val2tile_real(realpos);


        textprintf_ex(buffer, font, 200, SCREEN_H - 30, makecol(0, 0, 0), -1, "Mouse: %i, %i", realtile.getX(), realtile.getY()); 

        if(realtile.getX() > 5 && realtile.getY() > 5)
        textprintf_ex(buffer, font, 300, SCREEN_H - 30, makecol(0, 0, 0), -1, "Thrive: %i", client->bman.thrive_value_get(client->map, client->player_me->slot_get(), realtile.getX(), realtile.getY()));

        // Draw console:
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
    
    // Draw mouse pointer:
    masked_blit(mouse_pointer, buffer, 0, 0, mouse.getPosition().getX(), mouse.getPosition().getY(), 32, 32);

}


void GUI::update()
{

    mouse.update();    

    if(client->state_menu) {



    } else if(client->state_game) {
    
        // Mouse input:
        if(mouse.getLeftButtonState() == STATE_PRESS) {
        
            mouse_down_tile = mouse.getPressPosition();
            mouse_down_tile.translate(client->cam);
            mouse_down_tile = client->map->val2tile_real(mouse_down_tile);
                    
            std::cout << "mouse press event" << std::endl;
            
        }
        else if(mouse.getLeftButtonState() == STATE_RELEASE) {    
        
            mouse_up_tile = mouse.getPressPosition();
            mouse_up_tile.translate(client->cam);
            mouse_up_tile = client->map->val2tile_real(mouse_up_tile);        
            std::cout << "mouse release event" << std::endl;
             
            Point::fix_points(mouse_down_tile, mouse_up_tile);
            
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

        if(key[KEY_UP]    || mouse_y < 15           )camera.step(DIR_UP,    3, client->map->getWidth() * TILE_W / 2, client->map->getHeight() * TILE_H / 2);        
        if(key[KEY_RIGHT] || mouse_x > SCREEN_W - 15)camera.step(DIR_RIGHT, 3, client->map->getWidth() * TILE_W / 2, client->map->getHeight() * TILE_H / 2);
        if(key[KEY_DOWN]  || mouse_y > SCREEN_H - 15)camera.step(DIR_DOWN,  3, client->map->getWidth() * TILE_W / 2, client->map->getHeight() * TILE_H / 2);
        if(key[KEY_LEFT]  || mouse_x < 15           )camera.step(DIR_LEFT,  3, client->map->getWidth() * TILE_W / 2, client->map->getHeight() * TILE_H / 2);
    
        if(keypressed()) {
            if(key[KEY_ESC])client->state_running = false;
            if(key[KEY_F1])console_show = !console_show;

            if(key[KEY_PLUS_PAD])tool++;
            if(key[KEY_MINUS_PAD])tool--;
            
              


            
            clear_keybuf();
        }

    }    



}


void GUI::console_log(std::string s)
{
    console_data.push_back(s);
}

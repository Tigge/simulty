#include "gui.h"
#include "client.h"
#include "allegro.h"





gui::gui (  ){


    menu_background = load_bitmap("img/menubg.pcx", NULL);

    console_show = false;

}

gui::~gui (  ){
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
    
    
        rectfill(buffer, 0, SCREEN_H - 50, SCREEN_W, SCREEN_H, makecol(200, 200, 200));

        textprintf_ex(buffer, font, 20, SCREEN_H - 40, makecol(0, 0, 0), -1, "Money: %i", client->money); 
        textprintf_ex(buffer, font, 20, SCREEN_H - 30, makecol(0, 0, 0), -1, "Time: %i", client->time); 
        textprintf_ex(buffer, font, 20, SCREEN_H - 20, makecol(0, 0, 0), -1, "Tool: %i", client->tool); 

        textprintf_ex(buffer, font, 200, SCREEN_H - 20, makecol(0, 0, 0), -1, "FPS: %i", client->fps); 

        point mouse    = client->mouse->pos; point cam = client->cam;
        point realpos  = point(mouse.x + cam.x, mouse.y + cam.y);
        point realtile = client->m->val2tile_real(realpos);


        textprintf_ex(buffer, font, 200, SCREEN_H - 30, makecol(0, 0, 0), -1, "Mouse: %i, %i", realtile.x, realtile.y); 


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

}


void gui::update()
{
    if(client->state_menu)
    {



    }
    else if(client->state_game)
    {
        if(keypressed()) {
            if(key[KEY_ESC])client->state_running = false;
            if(key[KEY_F1])console_show = !console_show;

            if(key[KEY_PLUS_PAD])client->tool++;
            if(key[KEY_MINUS_PAD])client->tool--;
            
            clear_keybuf();
        }

    }    



}


void gui::console_log(std::string s)
{
    console_data.push_back(s);
}

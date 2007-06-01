

#ifndef _GUI_H_
#define _GUI_H_

//#include <guichan.hpp>
//#include <guichan/allegro.hpp>
#include "allegro.h"
#ifdef WIN32
  #include "winalleg.h"
#endif

#include <vector>

#include "Camera.hpp"
#include "Mouse.hpp"

#include "MapRender.hpp"
#include "BuildingRender.hpp"

const unsigned char SIMULTY_CLIENT_TOOL_NONE           = 0;
const unsigned char SIMULTY_CLIENT_TOOL_LAND           = 1;
const unsigned char SIMULTY_CLIENT_TOOL_ROAD           = 2;

const unsigned char SIMULTY_CLIENT_TOOL_ZONE_RES       = 10;
const unsigned char SIMULTY_CLIENT_TOOL_ZONE_COM       = 11;
const unsigned char SIMULTY_CLIENT_TOOL_ZONE_IND       = 12;

const unsigned char SIMULTY_CLIENT_TOOL_BUILD_POLICE   = 50;
const unsigned char SIMULTY_CLIENT_TOOL_BUILD_HOSPITAL = 51;
const unsigned char SIMULTY_CLIENT_TOOL_BUILD_FIRE     = 52;

class Client;

class GUI {
  private:

    BITMAP *buffer;

    int fps;
    int speed_counter;
    int frames;

    // Guichan:
    /*
    gcn::AllegroInput       *input;
    gcn::AllegroGraphics    *graphics;
    gcn::AllegroImageLoader *imageLoader;
    
    gcn::Gui        *gui;
    gcn::Container  *top;
    gcn::ImageFont  *guiFont;
    */

    BITMAP *menu_background;
    BITMAP *gui_background;
    
    BITMAP *mouse_block;   
    BITMAP *mouse_pointer;     
    
    BITMAP *icon_ind;
    BITMAP *icon_res;
    BITMAP *icon_com;
    
    BITMAP *icon_road;
    BITMAP *icon_land;
    
    BITMAP *icon_police;
    BITMAP *icon_fire;
    BITMAP *icon_hospital;
    
    bool                     console_show;
    std::vector<std::string> console_data;
        
    Mouse  mouse;
    Camera camera;
    
    Client         *client;
    
    MapRender       *mr;
    BuildingRender  *br;
    
    Point           mouse_down_tile;
    Point           mouse_up_tile;
    
    int tool;    
        
  public:

    std::string test;

    void console_log(std::string s);
    
    bool pointInArea(Point p, Point a, Point b);
  
    GUI();
    ~GUI();
    void render();
    void update();

    bool needUpdate();
    bool running();

    static void handleSpeed(void *);
    static void handleFPS(void *);
};

#endif

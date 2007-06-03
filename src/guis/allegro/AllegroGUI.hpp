

#ifndef _ALLEGROGUI_H_
#define _ALLEGROGUI_H_

#include <guichan.hpp>
#include <guichan/allegro.hpp>
#include "allegro.h"
#ifdef WIN32
  #include "winalleg.h"
#endif

#include <vector>

#include "../../GUI.hpp"

#include "widgets/ImageButton.hpp"

#include "Camera.hpp"
#include "Mouse.hpp"

#include "MapRender.hpp"
#include "BuildingRender.hpp"



class AllegroGUI : public GUI, gcn::KeyListener {
  private:

    BITMAP *buffer;

    int fps;
    int speed_counter;
    int frames;

    // Guichan:
    
    gcn::AllegroInput       *input;
    gcn::AllegroGraphics    *graphics;
    gcn::AllegroImageLoader *imageLoader;
    
    gcn::Gui          *gui;
    gcn::Container    *top;
    //gcn::DefaultFont  *guiFont;
    
    gcn::Button     *indButton;
    gcn::Button     *resButton;
    gcn::Button     *comButton;
    
    gcn::Image      *indIcon;
    gcn::Image      *resIcon;
    gcn::Image      *comIcon;

    BITMAP *menu_background;
    BITMAP *gui_background;
    
    BITMAP *mouse_block;   
    BITMAP *mouse_pointer;     

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

    virtual void keyPressed(gcn::KeyEvent &keyEvent);
    virtual void keyReleased(gcn::KeyEvent &keyEvent);

    std::string test;

    void console_log(std::string s);
    
    bool pointInArea(Point p, Point a, Point b);
  
    AllegroGUI();
    ~AllegroGUI();
    void render();
    void update();

    bool needUpdate();
    bool running();

    static void handleSpeed(void *);
    static void handleFPS(void *);
};

#endif



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
#include "MiniMap.hpp"



class AllegroGUI : public GUI, gcn::KeyListener, gcn::ActionListener, 
    gcn::MouseListener {
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

    gcn::Button      *roadButton;
    gcn::Button      *landButton;
    
    gcn::Button      *policeButton;
    gcn::Button      *fireButton;
    gcn::Button      *hospitalButton;
    
    gcn::Button      *bulldozerButton;

    gcn::Image      *indIcon;
    gcn::Image      *resIcon;
    gcn::Image      *comIcon;

    gcn::Image      *roadIcon;
    gcn::Image      *landIcon;
    
    gcn::Image      *policeIcon;
    gcn::Image      *fireIcon;
    gcn::Image      *hospitalIcon;
    gcn::Image      *bulldozerIcon;


    BITMAP *menu_background;
    BITMAP *gui_background;
    
    BITMAP *mouse_block;   
    BITMAP *mouse_pointer;     


    
    bool                     console_show;
    std::vector<std::string> console_data;
        
    Mouse  mouse;
    Camera camera;
    
    Client         *client;
    
    MapRender       *mr;
    BuildingRender  *br;
    
    MiniMap         *miniMap;
    
    Point           mouse_down_tile;
    Point           mouse_up_tile;
    
    bool usingTool;
    int  tool;
    
    int scrollSpeed;
        
  public:

    virtual void keyPressed(gcn::KeyEvent &keyEvent);
    virtual void keyReleased(gcn::KeyEvent &keyEvent);

    virtual void mouseDragged (gcn::MouseEvent &mouseEvent);

    virtual void mousePressed (gcn::MouseEvent &mouseEvent);
    virtual void mouseReleased (gcn::MouseEvent &mouseEvent);



    virtual void action(const gcn::ActionEvent &actionEvent);

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

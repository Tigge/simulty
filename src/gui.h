

#ifndef _GUI_H_
#define _GUI_H_

#include "allegro.h"

#ifdef WIN32
  #include "winalleg.h"
#endif

#include <vector>

#include "mouse_handler.h"

const unsigned char SIMULTY_CLIENT_TOOL_NONE           = 0;
const unsigned char SIMULTY_CLIENT_TOOL_LAND           = 1;
const unsigned char SIMULTY_CLIENT_TOOL_ROAD           = 2;

const unsigned char SIMULTY_CLIENT_TOOL_ZONE_RES       = 10;
const unsigned char SIMULTY_CLIENT_TOOL_ZONE_COM       = 11;
const unsigned char SIMULTY_CLIENT_TOOL_ZONE_IND       = 12;

const unsigned char SIMULTY_CLIENT_TOOL_BUILD_POLICE   = 50;
const unsigned char SIMULTY_CLIENT_TOOL_BUILD_HOSPITAL = 51;
const unsigned char SIMULTY_CLIENT_TOOL_BUILD_FIRE     = 52;

class gui {
  private:

    BITMAP *menu_background;
    BITMAP *gui_background;
    
    BITMAP *mouse_block;   
    BITMAP *mouse_pointer;     
    
    BITMAP *icon_ind;
    BITMAP *icon_res;
    BITMAP *icon_com;    
    
    bool                    console_show;
    std::vector<std::string> console_data;
        
    mouse_handler *mouse;
    
    Point mouse_down_tile;
    Point mouse_up_tile;
    
    int tool;    
        
  public:
  
    void console_log(std::string s);
  
    gui (  );
    ~gui (  );
    void render( BITMAP * );
    void update();
};

#endif

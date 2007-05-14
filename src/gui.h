

#ifndef _GUI_H_
#define _GUI_H_


#include <vector>

#include "allegro.h"
#include "mouse_handler.h"


class gui {
  private:

    BITMAP *menu_background;
    
    BITMAP *mouse_block;   
    BITMAP *mouse_pointer;     
    
    bool                    console_show;
    std::vector<std::string> console_data;
        
    mouse_handler *mouse;
    
    point mouse_down_tile;
    point mouse_up_tile;
    
    int tool;    
        
  public:
  
    void console_log(std::string s);
  
    gui (  );
    ~gui (  );
    void render( BITMAP * );
    void update();
};

#endif

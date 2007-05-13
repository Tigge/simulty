

#ifndef _GUI_H_
#define _GUI_H_


#include <vector>

#include "allegro.h"


class gui {
  private:

    BITMAP *menu_background;
    
    bool                    console_show;
    std::vector<std::string> console_data;
  public:
  
    void console_log(std::string s);
  
    gui (  );
    ~gui (  );
    void render( BITMAP * );
    void update();
};

#endif

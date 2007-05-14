#include "mouse_handler.h"


mouse_handler::mouse_handler()
{
    pos.x = mouse_x;
    pos.y = mouse_y;


    button_left   = EVENT_NONE;
    button_middle = EVENT_NONE;
    button_right  = EVENT_NONE;

}

void mouse_handler::update()
{

    pos.x = mouse_x;
    pos.y = mouse_y;

    // TODO: middle and right buttons

    // Change to long state:
    if     (button_left == EVENT_PRESS  ) {
        button_left = EVENT_HOLD; 
    } else if(button_left == EVENT_RELEASE) {
        button_left = EVENT_NONE;
    // Check if we want to continue state:
    } else if( mouse_b & 1 && button_left == EVENT_NONE) {
       button_left = EVENT_PRESS;
       down = pos;
    } else if(!mouse_b & 1 && button_left == EVENT_HOLD  ) {
      button_left = EVENT_RELEASE;
      up = pos;
    }


}

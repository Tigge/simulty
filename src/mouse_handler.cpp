#include "mouse_handler.h"


mouse_handler::mouse_handler()
{
    pos.x = mouse_x;
    pos.y = mouse_y;


    button_left   = BUTTON_IS_UP;
    button_middle = BUTTON_IS_UP;
    button_right  = BUTTON_IS_UP;

}

void mouse_handler::update()
{

    pos.x = mouse_x;
    pos.y = mouse_y;

    // Change to long state:
    if     (button_left == BUTTON_DOWN)button_left = BUTTON_IS_DOWN;
    else if(button_left == BUTTON_UP  )button_left = BUTTON_IS_UP;
    // Check if we want to continue state:
    else if( mouse_b & 1 && button_left == BUTTON_IS_UP  )button_left = BUTTON_DOWN;
    else if(!mouse_b & 1 && button_left == BUTTON_IS_DOWN)button_left = BUTTON_UP;


}

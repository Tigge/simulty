
#ifndef _MOUSE_HANDLER_H_
#define _MOUSE_HANDLER_H_

#include "allegro.h"
#include "point.h"

const unsigned char BUTTON_IS_UP    = 0;
const unsigned char BUTTON_DOWN     = 1;
const unsigned char BUTTON_IS_DOWN  = 2;
const unsigned char BUTTON_UP       = 3;

class mouse_handler
{
    public:

    point pos;

    unsigned char button_left;
    unsigned char button_middle;
    unsigned char button_right;

    mouse_handler();

    void update();

};

#endif

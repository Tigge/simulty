
#ifndef _MOUSE_HANDLER_H_
#define _MOUSE_HANDLER_H_

#include "allegro.h"
#include "Point.hpp"

const unsigned char EVENT_NONE    = 0; // On "normal"
const unsigned char EVENT_PRESS   = 1; // On press
const unsigned char EVENT_HOLD    = 2; // On hold
const unsigned char EVENT_RELEASE = 3; // On release

class mouse_handler
{
    public:

    Point pos;
    
    Point up;
    Point down;

    unsigned char button_left;
    unsigned char button_middle;
    unsigned char button_right;

    mouse_handler();

    void update();

};

#endif

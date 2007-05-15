
#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "allegro.h"
#include "Point.hpp"

const unsigned char STATE_NONE    = 0; // On "normal"
const unsigned char STATE_PRESS   = 1; // On press
const unsigned char STATE_HOLD    = 2; // On hold
const unsigned char STATE_RELEASE = 3; // On release

class Mouse {

    private:

    // Positions:
    Point position;    
    Point releasePosition;
    Point pressPosition;

    // Button states:
    unsigned char leftButton;
    unsigned char middleButton;
    unsigned char rightButton;

    public:

    Mouse();
    
    Point getPosition();
    Point getPressPosition();
    Point getReleasePosition();
    
    unsigned char getLeftButtonState();

    void update();

};

#endif

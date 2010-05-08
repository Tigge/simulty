#include "Mouse.hpp"

#include <iostream>

Mouse::Mouse() {

    position.setX(mouse_x);
    position.setY(mouse_y);


    leftButton   = STATE_NONE;
    middleButton = STATE_NONE;
    rightButton  = STATE_NONE;

}

    
Point Mouse::getPosition() {
    return position;
}
Point Mouse::getPressPosition() {
    return pressPosition;
}
Point Mouse::getReleasePosition() {
    return releasePosition;
}
unsigned char Mouse::getLeftButtonState() {
    return leftButton;
}

void Mouse::update() {

    position.setX(mouse_x);
    position.setY(mouse_y);

    // TODO: middle and right buttons

    // Change to long state:
    if (leftButton == STATE_PRESS) {
        leftButton = STATE_HOLD; 
    } else if(leftButton == STATE_RELEASE) {
        leftButton = STATE_NONE;
    // Check if we want to continue state:
    } else if(mouse_b & 1 && leftButton == STATE_NONE) {
        leftButton = STATE_PRESS;
        pressPosition = position;
    } else if(!mouse_b & 1 && leftButton == STATE_HOLD) {
        leftButton = STATE_RELEASE;
        releasePosition = position;
    }

}

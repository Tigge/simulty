
#include "../../shared.h"
#include "Camera.hpp"

Camera::Camera() : Point(0, 0) {

}


void Camera::step(const unsigned char direction, int steps, int width, int height) {

    if(direction == DIR_UP && getY() - steps >= 0) { 
        translate(0, -steps);
    } else if(direction == DIR_RIGHT && getX() + steps < width) { 
        translate(steps, 0);
    } else if(direction == DIR_DOWN && getY() + steps < height) { 
        translate(0, steps);
    } else if(direction == DIR_LEFT && getX() - steps >= 0) {
        translate(-steps, 0);
    }

}

#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "../../Point.hpp"

class Camera : public Point {

    // TODO: acceleration, etc

    public:

    Camera();

    void step(const unsigned char direction, int steps, int width, int height);

};


#endif


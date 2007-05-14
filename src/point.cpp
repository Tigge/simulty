#include "point.h"

point::point()
{
    x = y = 0;
}

point::point(int nx, int ny)
{
    x = nx;
    y = ny;
}


void point::translate(int x, int y) {
  this->x += x;
  this->y += y;
}

void point::translate(point p) {
  translate(p.x, p.y);

}
    
void point::fix_points(point &a, point &b) {

    if(a.x > b.x) {
        int tmp = a.x;
        a.x = b.x;
        b.x = tmp;            
    } if(a.y > b.y) {
        int tmp = a.y;
        a.y = b.y;
        b.y = tmp;
    }
}


#include "Point.hpp"


// Constructors:

Point::Point() {
  Point(0, 0);
}

Point::Point(int x, int y) {
    setX(x);
    setY(y);
}

// Getters and setters:

void Point::setX(int x) {
    this->x = x;
}

void Point::setY(int y) {
    this->y = y;
}

int Point::getX() {
    return this->x;
}

int Point::getY() {
    return this->y;
}

// Functions:

void Point::translate(int x, int y) {
   setX(getX() + x);
   setY(getY() + y);
}

void Point::translate(Point p) {
    translate(p.getX(), p.getY());
}
    
    
void Point::move(int x, int y) {
    setX(x);
    setY(y);
}

void Point::move(Point p) {
    move(p.getX(), p.getY());
}
    
void Point::fix_points(Point &a, Point &b) {

    if(a.getX() > b.getX()) {
        int tmp = a.getX();
        a.setX(b.getX());
        b.setX(tmp);
    } if(a.getY() > b.getY()) {
        int tmp = a.getY();
        a.setY(b.getY());
        b.setY(tmp);
    }
}


std::ostream& operator<<(std::ostream& output, const Point& p) {
    output << "(" <<  p.x << ", " << p.y << ")";
    return output;
}

Point Point::operator+(Point p) {
    return Point(getX() + p.getX(), getY() + p.getY());
}

Point Point::operator-(Point p) {
    return Point(getX() - p.getX(), getY() - p.getY());
}


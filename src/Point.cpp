#include "Point.hpp"


// Constructors:

Point::Point() {
  setX(0);
  setY(0);
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

int  Point::distance(Point a, Point b) {

   float dx = a.getX() - b.getX();
   float dy = a.getY() - b.getY();

  return round(sqrt(pow(dx, 2.0) + pow(dy, 2.0)));
}

void Point::fixOrder(Point &a, Point &b) {

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

bool Point::inArea(Point p, Point a, int w, int h) {

  if(p.getX() >= a.getX() && p.getX() <= a.getX() + w
      && p.getY() >= a.getY() && p.getY() <= a.getY() + h)
    return true;
  else
    return false;

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

Point Point::fromPacket(NLPacket &p) {
  int a = p.nextInt32();
  int b = p.nextInt32();
  return Point(a, b);
}

void Point::unsign() {
  if(x < 0) x = 0;
  if(y < 0) y = 0;
}

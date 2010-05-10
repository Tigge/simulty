#ifndef _POINT_HPP_
#define _POINT_HPP_

#include <ostream>
#include <cmath>

#include "NL.hpp"

/**
 * Simple class representation of a point.
 *
 * Etc.. more info.
 *
 */
class Point {

  private:

  int x;
  int y;

  public:

  Point();
  Point(int x, int y);

  /**
    * Getter for the x value.
    *
    * @return the x value
    */
  int getX();

  /**
    * Getter for the y value.
    *
    * @return the y value
    */
  int getY();

  /**
    * Setter for the x value.
    *
    * @param x the new x value
    */
  void setX(int x);

  /**
    * Setter for the y value.
    *
    * @param y the new y value
    */
  void setY(int y);


  /**
    * Translates (adds) the x and y value to the current point
    *
    * @param x the x value to add
    * @param y the x value to add
    */
  void translate(int x, int y);
  
  /**
    * Translates (adds) the given point to the current point.
    *
    * This method just calls the translate(int x, int y) functions.
    *
    * @param p the point to add
    * @see     translate(int x, int y)
    */
  void translate(Point p);

  /**
    * Moves the point to a new x and y value
    *
    * @param x the new x value
    * @param y the new y value
    */
  void move(int x, int y);
  /**
    * Moves the point to a new point
    *
    * @param p the point to move to
    */
  void move(Point p);

  /**
    * Calculates distance from point a to point b
    *
    * @param a point a
    * @param b point b
    */
  static int  distance(Point a, Point b);

  static void fixOrder(Point &a, Point &b);
  bool inArea(Point a, Point b);
  void unsign();

  static Point fromPacket(NL::Packet &);

  // For output in streams:
  friend std::ostream& operator<<(std::ostream& output, const Point& p);

  // Addition and substraction of points
  Point operator+(Point p);
  Point operator-(Point p);

};

#endif


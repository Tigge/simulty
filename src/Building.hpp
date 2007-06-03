#ifndef _BUILDING_HPP_
#define _BUILDING_HPP_

#include <string>


#include "Point.hpp"

class Building {
  // Associations
  // Attributes

  public:

    static const unsigned char TYPE_POLICE      = 0;
    static const unsigned char TYPE_FIRE        = 1;
    static const unsigned char TYPE_HOSPITAL    = 2;
    
    static const unsigned char TYPE_RESIDENTIAL = 3;
    static const unsigned char TYPE_COMMERSIAL  = 4;
    static const unsigned char TYPE_INDUSTRIAL  = 5;

  public:
    std::string name;
    int owner;
    int cost;
    int maintenace;
    int income;
    int expence;
    unsigned char width;
    unsigned char height;

    Point position;
  // Operations
  public:

    Building(Point position, int owner);
    virtual ~Building();

    virtual Point getPosition();

    virtual unsigned char getType() = 0;
    virtual unsigned char getWidth();
    virtual unsigned char getHeight();

    virtual void setWidth(unsigned char set_width);
    virtual void setHeight(unsigned char set_height);

    virtual int getOwner();

    virtual int getZ();

    virtual void render();
    virtual void update();
};

#endif

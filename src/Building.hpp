#ifndef _BUILDING_HPP_
#define _BUILDING_HPP_

#include <string>

#include "Date.hpp"
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
    unsigned char owner;
    //int cost;
    //int maintenace;
    //int income;
    //int expence;
    unsigned char width;
    unsigned char height;

    Point position;
    
    Date built;
    // Operations
  public:

    Building(Point position, unsigned char owner, int w, int h, Date built);
    virtual ~Building();

    virtual Point getPosition();

    virtual unsigned char getType() = 0;
    virtual unsigned char getWidth();
    virtual unsigned char getHeight();

    virtual void setWidth(unsigned char width);
    virtual void setHeight(unsigned char height);

    virtual int getOwner();

    virtual unsigned int getZ();

    virtual void render();
    virtual void update();
};

#endif

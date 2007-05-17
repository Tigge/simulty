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
  
    Building(Point position);
    virtual ~Building();
    
    virtual Point getPosition();
    
    virtual unsigned char getType() = 0;
    virtual unsigned char getWidth();
    virtual unsigned char getHeight();
    
    virtual void render();
    virtual void update();

};

#endif

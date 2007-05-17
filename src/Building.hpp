#ifndef _BUILDING_HPP_
#define _BUILDING_HPP_

#include <string>

const unsigned char SIMULTY_BUILDING_POLICE           = 0;
const unsigned char SIMULTY_BUILDING_FIRE             = 1;
const unsigned char SIMULTY_BUILDING_HOSPITAL         = 2;


class Building {
  // Associations
  // Attributes
  public:
    std::string name;
    int owner;
    int cost;
    int maintenace;
    int income;
    int expence;
    unsigned char width;
    unsigned char height;
    int x;
    int y;
  // Operations
  public:
  
    Building(int x, int y);
    virtual ~Building();
    
    virtual void render();
    virtual void update();

};

#endif

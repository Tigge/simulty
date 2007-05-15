#ifndef _BUILDING_HPP_
#define _BUILDING_HPP_

#include <string>

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
    unsigned char size_x;
    unsigned char size_y;
    int pos_x;
    int pos_y;
  // Operations
  public:
    Building (  );
    virtual ~Building (  );
    
    virtual void render (  );
    virtual void update (  );

};

#endif

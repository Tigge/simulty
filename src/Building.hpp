#ifndef BUILDING_H
#define BUILDING_H

#include <string>


class building {
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
    building (  );
    virtual ~building (  );
    
    virtual void render (  );
    virtual void update (  );

};

#endif

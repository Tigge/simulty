#ifndef BUILDING_H
#define BUILDING_H

#include <string>

using namespace std;

class building {
  // Associations
  // Attributes
  public:
    string name;
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
    ~building (  );
    void render (  );
    void update (  );
};

#endif

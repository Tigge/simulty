#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "NL.hpp"

const int PLAYER_TYPE_BASE = 0;

class Player {
  // Associations
  // Attributes
  protected:
    std::string nick;

    int id;
    int slot;

    int money;
    int tax;
    int state;
    int type;
    int expences_year;
    int expences_last;
    int level_electricity;
    int level_police;
    int level_fire;
    int level_education;

  // Operations
  public:
  
    virtual int id_get();  
    virtual int slot_get();
    virtual int type_get();

    virtual int  money_get();
    virtual void money_set(int m);

    virtual void update();

    Player(NLINT32, NLINT16);
    virtual ~Player ();


};

#endif
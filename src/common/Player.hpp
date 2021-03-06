#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "NL.hpp"

#include "Budget.hpp"
#include "ThriveMap.hpp"

const int PLAYER_TYPE_BASE = 0;

class Player {
  // Associations
  // Attributes
  protected:
    std::string nick;

    int id;
    int slot;

    //int money;
    //int tax;
    int state;
    int type;
    //int expences_year;
    //int expences_last;
    
    Budget    *budget;
    
    ThriveMap *thriveMap;
    
    /*int level_electricity;
    int level_police;
    int level_fire;
    int level_education; */

  // Operations
  public:

    Player(NLINT32, NLINT16);
    virtual ~Player ();

    virtual int        getId();
    virtual int        getSlot();
    virtual int        getType();

    //virtual int        getTax();
    //virtual void       setTax(unsigned int tax);

    //virtual int        getMoney();
    //virtual void       setMoney(int m);

    virtual void       setThriveMap(ThriveMap *);
    virtual ThriveMap *getThriveMap();
    
    virtual void       setBudget(Budget *);
    virtual Budget    *getBudget();
  
    virtual void       update();
};

#endif

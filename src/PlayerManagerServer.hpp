#ifndef PLAYER_MANAGER_SERVER_H
#define PLAYER_MANAGER_SERVER_H

#include "PlayerManager.hpp"

class PlayerManagerServer : public PlayerManager {

    //TODO, add slot information

    public:
   
    PlayerManagerServer();
    virtual ~PlayerManagerServer();
  
    virtual int  nextSlot();
    
    //virtual void changeMoney(int slot, int money);
};

#endif

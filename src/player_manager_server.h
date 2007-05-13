#ifndef PLAYER_MANAGER_SERVER_H
#define PLAYER_MANAGER_SERVER_H

#include "player_manager.h"

class player_manager_server : public player_manager 
{
    public:
   
    player_manager_server();
    virtual ~player_manager_server();
  
    virtual int  slot_next();
};

#endif

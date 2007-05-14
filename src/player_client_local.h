#ifndef PLAYER_CLIENT_LOCAL_H
#define PLAYER_CLIENT_LOCAL_H

#include "player.h"

const int PLAYER_TYPE_CLIENT_LOCAL = 1;

class player_client_local: public player {
  // Associations
  // Attributes
  // Operations
  public:
    player_client_local(NL_INT32, NL_INT16);
    //player_client_local();
    virtual ~player_client_local();
    void update();
};

#endif

#ifndef PLAYER_CLIENT_LOCAL_H
#define PLAYER_CLIENT_LOCAL_H

#include "Player.hpp"

const int PLAYER_TYPE_CLIENT_LOCAL = 1;

class player_client_local: public Player {
  // Associations
  // Attributes
  // Operations
  public:
    player_client_local(NLINT32, NLINT16);
    //player_client_local();
    virtual ~player_client_local();
    void update();
};

#endif

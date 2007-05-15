#ifndef PLAYER_CLIENT_REMOTE_H
#define PLAYER_CLIENT_REMOTE_H

#include "player.h"

#include "NL.hpp"

const int PLAYER_TYPE_CLIENT_REMOTE = 2;

class player_client_remote: public Player {
  // Associations
  // Attributes
  public:
     NLSocket *socket;
     int status; // no status (0) connected (1), authenticated(2), disconnected (1000 - 1600)
  // Operations
  public:
    player_client_remote(NLINT32, NLINT16);
    virtual ~player_client_remote();
    void update();
};

#endif

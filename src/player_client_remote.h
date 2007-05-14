#ifndef PLAYER_CLIENT_REMOTE_H
#define PLAYER_CLIENT_REMOTE_H

#include "player.h"

#include "netlib.h"

const int PLAYER_TYPE_CLIENT_REMOTE = 2;

class player_client_remote: public player {
  // Associations
  // Attributes
  public:
     NSocket *socket;
     int status; // no status (0) connected (1), authenticated(2), disconnected (1000 - 1600)
  // Operations
  public:
    player_client_remote(NL_INT32, NL_INT16);
    virtual ~player_client_remote();
    void update();
};

#endif

#ifndef PLAYER_CLIENT_REMOTE_H
#define PLAYER_CLIENT_REMOTE_H

#include "Player.hpp"

#include "NL.hpp"

const int PLAYER_TYPE_CLIENT_REMOTE = 2;

class PlayerClientRemote: public Player {
  // Associations
  // Attributes
  public:
     NL::Socket *socket;
     int status; // no status (0) connected (1), authenticated(2), disconnected (1000 - 1600)
  // Operations
  public:
    PlayerClientRemote(NLINT32, NLINT16);
    virtual ~PlayerClientRemote();
    void update();
};

#endif

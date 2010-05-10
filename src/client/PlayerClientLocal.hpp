#ifndef PLAYER_CLIENT_LOCAL_H
#define PLAYER_CLIENT_LOCAL_H

#include "Player.hpp"

const int PLAYER_TYPE_CLIENT_LOCAL = 1;

class PlayerClientLocal: public Player {
  // Associations
  // Attributes
  // Operations
  public:
    PlayerClientLocal(NLINT32, NLINT16);
    virtual ~PlayerClientLocal();
    void update();
};

#endif

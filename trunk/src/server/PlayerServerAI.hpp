#ifndef PLAYER_SERVER_AI_H
#define PLAYER_SERVER_AI_H

#include "Player.hpp"

const int PLAYER_TYPE_SERVER_AI = 10;

class PlayerServerAI: public Player {
  // Associations
  // Attributes
  // Operations
  public:
    PlayerServerAI(NLINT32, NLINT16);
    ~PlayerServerAI();
    void update();
};

#endif

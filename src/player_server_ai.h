#ifndef PLAYER_SERVER_AI_H
#define PLAYER_SERVER_AI_H

#include "player.h"

const int PLAYER_TYPE_SERVER_AI = 10;

class player_server_ai: public player {
  // Associations
  // Attributes
  // Operations
  public:
    player_server_ai(INT32, INT16);
    ~player_server_ai (  );
    void update (  );
};

#endif

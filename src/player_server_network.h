#ifndef PLAYER_SERVER_NETWORK_H
#define PLAYER_SERVER_NETWORK_H

#include "shared.h"
#include "NL.hpp"
#include "Player.hpp"

const int PLAYER_TYPE_SERVER_NETWORK = 3;

class player_server_network: public Player {
  // Associations
  // Attributes
  public:
     NLSocket *socket;
     int status; // no status (0) connected (1), authenticated(2), disconnected (1000 - 1600)
  // Operations
  public:
  
    virtual void money_set(int money_new);
  
    player_server_network(NLINT32, NLINT16);
    virtual ~player_server_network (  );
    void update (  );
};

#endif

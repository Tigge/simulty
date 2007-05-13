#ifndef PLAYER_SERVER_NETWORK_H
#define PLAYER_SERVER_NETWORK_H

#include "shared.h"
#include "netlib.h"
#include "player.h"

const int PLAYER_TYPE_SERVER_NETWORK = 3;

class player_server_network: public player {
  // Associations
  // Attributes
  public:
     NSocket *socket;
     int status; // no status (0) connected (1), authenticated(2), disconnected (1000 - 1600)
  // Operations
  public:
  
    virtual void money_set(int money_new);
  
    player_server_network(INT32, INT16);
    virtual ~player_server_network (  );
    void update (  );
};

#endif

#ifndef PLAYER_SERVER_NETWORK_H
#define PLAYER_SERVER_NETWORK_H

#include "shared.h"
#include "NL.hpp"

#include "Player.hpp"
#include "PacketReceiver.hpp"
#include "PacketSender.hpp"

const int PLAYER_TYPE_SERVER_NETWORK = 3;

class PlayerServerNetwork: public Player {
  // Associations
  // Attributes
  public:
     NL::Socket     *socket;
     PacketReceiver *receiver;
     PacketSender   *sender;
          
     int status; // no status (0) connected (1), authenticated(2), disconnected (1000 - 1600)
  // Operations
  public:

    //virtual void setMoney(int money_new);

    PlayerServerNetwork(NLINT32, NLINT16, NL::Socket *);
    virtual ~PlayerServerNetwork();
    void update();
};

#endif

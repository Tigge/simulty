#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include "netlib.h"

#include "map_base.h"

#include "player_manager_server.h"

#include "player.h"
#include "player_server_network.h"
#include "player_server_ai.h"

#include "point.h"


const unsigned char SIMULTY_SERVER_STATE_LOBBY      = 1;

const unsigned char SIMULTY_SERVER_STATE_GAME_START = 2;
const unsigned char SIMULTY_SERVER_STATE_GAME_ON    = 3;
const unsigned char SIMULTY_SERVER_STATE_GAME_END   = 4;



class game_server {
  // Associations
  // Attributes
  private:
    NNetwork net;
    map_base *m;

    bool player_add(unsigned char player_type);
    bool player_remove(player *);

    bool packet_handle(player_server_network *from, NPacket pack);
    void packet_send(player_server_network *to, NPacket pack);
    void packet_send_to_all(NPacket pack);

    player_manager_server pman;

    bool time_advance;

  public:
    NSocket *net_server; 
  // Operations
  public:

    game_server();
    virtual ~game_server();


    void static time_increment(void *);
    void update (  );
};

#endif

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include "allegro.h"

#ifdef WIN32
  #include "winalleg.h"
#endif

#include "NL.hpp"

#include "Map.hpp"

#include "PlayerManagerServer.hpp"

#include "Player.hpp"
#include "player_server_network.h"
#include "player_server_ai.h"

#include "BuildingManager.hpp"

#include "Point.hpp"

#include "Calendar.hpp"

const unsigned char SIMULTY_SERVER_STATE_LOBBY      = 1;

const unsigned char SIMULTY_SERVER_STATE_GAME_START = 2;
const unsigned char SIMULTY_SERVER_STATE_GAME_ON    = 3;
const unsigned char SIMULTY_SERVER_STATE_GAME_END   = 4;



class Server {
  // Associations
  // Attributes
  private:
    NLNetwork net;
    Map *map;

    bool player_add(unsigned char player_type);
    bool player_remove(Player *);

    bool packet_handle(player_server_network *from, NLPacket pack);
    void packet_send(player_server_network *to, NLPacket pack);
    void packet_send_to_all(NLPacket pack);

    PlayerManagerServer pman;

    BuildingManager bman;

    Calendar calendar;

    bool time_advance;
    int  speed;
    int last_rebuilt;

  public:
    NLSocket *net_server;
  // Operations
  public:

    Server();
    virtual ~Server();

    void setSpeed(int speed);
    int  getSpeed();


    void static time_increment(void *);
    void update (  );
};

extern Server *server;

#endif

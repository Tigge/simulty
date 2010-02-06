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

#include "BuildingManagerServer.hpp"

#include "Point.hpp"

#include "Date.hpp"

#include "shared.h"
#include "elog.h"

#include "BuildingFactory.hpp"

#include "LoaderSaver.hpp"

const unsigned char SIMULTY_SERVER_STATE_LOBBY      = 1;

const unsigned char SIMULTY_SERVER_STATE_GAME_START = 2;
const unsigned char SIMULTY_SERVER_STATE_GAME_ON    = 3;
const unsigned char SIMULTY_SERVER_STATE_GAME_END   = 4;



class Server {

  private:
  
  static Server* instance;
  
  NLNetwork      net;
  Map           *map;

  bool           player_add(unsigned char player_type);
  bool           player_remove(Player *);

  bool           packet_handle(player_server_network *from, NLPacket pack);

  PlayerManagerServer   pman;
  BuildingManagerServer bman;

  Date           date;

  bool           time_advance;
  int            speed;
  NLSocket      *net_server;

  void           updateBalance(Player *, unsigned char, unsigned char, int);

  protected:

  Server();
  Server(const Server&);
  Server& operator= (const Server&);

  public:

  virtual ~Server();
  static Server         *getInstance();

  void                   packet_send(player_server_network *to, NLPacket pack);
  void                   packet_send_to_all(NLPacket pack);

  void                   setSpeed(int speed);
  int                    getSpeed();
  
  Date                   getDate();
  
  PlayerManagerServer   *getPlayerManager();
  BuildingManagerServer *getBuildingManager();

  void static            time_increment(void *);
  void                   update();
  void                   updateThrive();
};

extern Server *server;

#endif

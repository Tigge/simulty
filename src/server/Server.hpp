#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include "SDL.h"

#include "NL.hpp"

#include "Map.hpp"

#include "PlayerManagerServer.hpp"

#include "Player.hpp"
#include "PlayerServerNetwork.hpp"
#include "PlayerServerAI.hpp"

#include "BuildingManagerServer.hpp"

#include "Point.hpp"

#include "Date.hpp"

#include "shared.h"
#include "elog.h"

#include "BuildingFactory.hpp"

#include "LoaderSaver.hpp"

#include "ServerListener.hpp"

const unsigned char SIMULTY_SERVER_STATE_LOBBY      = 1;

const unsigned char SIMULTY_SERVER_STATE_GAME_START = 2;
const unsigned char SIMULTY_SERVER_STATE_GAME_ON    = 3;
const unsigned char SIMULTY_SERVER_STATE_GAME_END   = 4;



class Server {

  private:
  
  static Server* instance;
  
  NL::Network    net;
  Map           *map;

  bool           player_add(unsigned char player_type, NL::Socket *);
  bool           player_remove(Player *);

  bool           packet_handle(PlayerServerNetwork *from, NL::Packet pack);

  PlayerManagerServer   pman;
  BuildingManagerServer bman;

  Date            date;

  int             speed;
  ServerListener *listener;

  void           updateBalance(Player *, unsigned char, unsigned char, int);

  SDL_TimerID    timerId;

  protected:

  Server();
  Server(const Server&);
  Server& operator= (const Server&);

  public:

  virtual ~Server();
  static Server         *getInstance();

  void                   packet_send(PlayerServerNetwork *to, NL::Packet pack);
  void                   packet_send_to_all(NL::Packet pack);

  void                   setSpeed(int speed);
  int                    getSpeed();
  
  Date                   getDate();
  
  PlayerManagerServer   *getPlayerManager();
  BuildingManagerServer *getBuildingManager();

  Uint32 static          time_increment(Uint32, void *);
  void                   update(SDL_Event *);
  void                   updateThrive();
  void                   advanceTime();
};

extern Server *server;

#endif

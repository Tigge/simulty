
#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include "SDL.h"

#include "SimultyException.hpp"

#include "PlayerManager.hpp"

#include "Player.hpp"
#include "PlayerClientLocal.hpp"
#include "PlayerClientRemote.hpp"

#include "BuildingManager.hpp"
#include "BuildingFactory.hpp"

#include "Date.hpp"

#include "Map.hpp"
#include "GUI.hpp"

#include <sstream>

const unsigned char SIMULTY_CLIENT_STATE_GAME_NONE  = 0;

const unsigned char SIMULTY_CLIENT_STATE_GAME_START = 2;
const unsigned char SIMULTY_CLIENT_STATE_GAME_ON    = 3;
const unsigned char SIMULTY_CLIENT_STATE_GAME_END   = 4;

const unsigned char SIMULTY_CLIENT_STATE_MENU_NONE  = 0;




class Client {
  // Attributes

  private:


    NL::Network net;

  public:

    Date      date;

    PlayerManager pman;
    PlayerClientLocal *myPlayer;

    BuildingManager bman;

    //bool cam_move_step(int dir, int step);
    //bool cam_move_jump(int x, int y);

    NL::Socket     *socket;

  public:

    Map *map;

    bool state_running;

    unsigned char state_menu;
    unsigned char state_game;

    //Point cam;
    GUI *gui;

    PacketSender   *sender;
    PacketReceiver *receiver;

  // Operations
  public:
    Client();
    ~Client();

    bool running();

    void init();
    void update();
    void render();    

    void packet_handle(NL::Packet p);

    void bulldoze(Point from, Point to);
    void deZone(Point from, Point to);

    void buyLand(Point from, Point to);
    void buyRoad(Point from, Point to);
    void buyZone(Point from, Point to, int type);
    void buyBuilding(Point where, int type);
    
    void debug(Point p);

    PlayerClientLocal *getMyPlayer();

};


#endif

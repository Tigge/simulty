
#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include "PlayerManager.hpp"

#include "Player.hpp"
#include "player_client_local.h"
#include "player_client_remote.h"

#include "BuildingManager.hpp"
#include "BuildingFactory.hpp"

#include "Calendar.hpp"

#include "Map.hpp"
#include "GUI.hpp"

const unsigned char SIMULTY_CLIENT_STATE_GAME_NONE  = 0;

const unsigned char SIMULTY_CLIENT_STATE_GAME_START = 2;
const unsigned char SIMULTY_CLIENT_STATE_GAME_ON    = 3;
const unsigned char SIMULTY_CLIENT_STATE_GAME_END   = 4;

const unsigned char SIMULTY_CLIENT_STATE_MENU_NONE  = 0;




class Client {
  // Attributes

  private:


    NLNetwork net;

  public:

    Calendar      cal;

    PlayerManager pman;
    player_client_local *myPlayer;

    BuildingManager bman;

    //bool cam_move_step(int dir, int step);
    //bool cam_move_jump(int x, int y);

  public:

    Map *map;

    bool state_running;

    unsigned char state_menu;
    unsigned char state_game;

    //Point cam;
    GUI *gui;

    NLSocket *net_client;

  // Operations
  public:
    Client(GUI *);
    
    ~Client();

    bool running();

    void update();

    void packet_handle(NLPacket p);

    void bulldoze(Point from, Point to);

    void buyLand(Point from, Point to);
    void buyRoad(Point from, Point to);
    void buyZone(Point from, Point to, int type);
    void buyBuilding(Point where, int type);

    player_client_local *getMyPlayer();

};


#endif

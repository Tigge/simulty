
#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_


#include "allegro.h"

#ifdef WIN32
  #include "winalleg.h"
#endif

#include "PlayerManager.hpp"

#include "Player.hpp"
#include "player_client_local.h"
#include "player_client_remote.h"

#include "BuildingManager.hpp"
#include "BuildingFactory.hpp"


#include "Map.hpp"
#include "GUIMap.hpp"

#include "GUI.hpp"

const unsigned char SIMULTY_CLIENT_STATE_GAME_NONE  = 0;

const unsigned char SIMULTY_CLIENT_STATE_GAME_START = 2;
const unsigned char SIMULTY_CLIENT_STATE_GAME_ON    = 3;
const unsigned char SIMULTY_CLIENT_STATE_GAME_END   = 4;

const unsigned char SIMULTY_CLIENT_STATE_MENU_NONE  = 0;




class Client {
  // Attributes

  private:

    BITMAP *buffer;    
    NLNetwork net;

  public:

    PlayerManager pman;
    player_client_local *player_me;

    BuildingManager bman;

    bool cam_move_step(int dir, int step);
    bool cam_move_jump(int x, int y);

  public:

    GUIMap *map;
    
    bool state_running;
    
    unsigned char state_menu;
    unsigned char state_game;

    int fps;
    int speed_counter;
    int frames;

    int time;
    int money;

    Point cam;

    NLSocket *net_client;
    GUI *gui;

  // Operations
  public:
    Client (  );
    ~Client (  );

    bool running();

    bool needupdate();

    void render (  );
    void update (  );

    void packet_handle(NLPacket p);
    
    void buy_land(Point from, Point to);
    void buy_road(Point from, Point to);
    void buy_zone(Point from, Point to, int type);
    void buy_building(Point where, int type);
    
};

extern Client *client;

#endif

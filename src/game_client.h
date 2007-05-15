
#ifndef _GAME_CLIENT_H_
#define _GAME_CLIENT_H_


#include "allegro.h"

#ifdef WIN32
  #include "winalleg.h"
#endif

#include "player_manager.h"

#include "player.h"
#include "player_client_local.h"
#include "player_client_remote.h"

#include "building_manager.h"


#include "map_base.h"
#include "map.h"

#include "gui.h"

const unsigned char SIMULTY_CLIENT_STATE_GAME_NONE  = 0;

const unsigned char SIMULTY_CLIENT_STATE_GAME_START = 2;
const unsigned char SIMULTY_CLIENT_STATE_GAME_ON    = 3;
const unsigned char SIMULTY_CLIENT_STATE_GAME_END   = 4;

const unsigned char SIMULTY_CLIENT_STATE_MENU_NONE  = 0;




class game_client {
  // Attributes

    private:

    BITMAP *buffer;
    
    NNetwork net;


  public:

    player_manager pman;
    player_client_local *player_me;

    building_manager bman;

    bool cam_move_step(int dir, int step);
    bool cam_move_jump(int x, int y);

  public:

    map *m;
    
    bool state_running;
    
    unsigned char state_menu;
    unsigned char state_game;

    int fps;
    int speed_counter;
    int frames;

    int time;


    int money;

    Point cam;

    NSocket *net_client;
    gui *g;

  // Operations
  public:
    game_client (  );
    ~game_client (  );

    bool running();

    bool needupdate();

    void render (  );
    void update (  );

    void packet_handle(NPacket p);
    
    void buy_land(Point from, Point to);
    void buy_road(Point from, Point to);
    void buy_zone(Point from, Point to, int type);
    void buy_building(Point where, int type);
    
};


#endif

#ifndef _SHARED_H_
#define _SHARED_H_

#include "NL.hpp"


//#define err std::cerr

const unsigned char SIMULTY_MAX_PLAYERS = 16;

// Directions:

const unsigned char DIR_NONE  = 0;
const unsigned char DIR_UP    = 1;
const unsigned char DIR_RIGHT = 2;
const unsigned char DIR_DOWN  = 3;
const unsigned char DIR_LEFT  = 4;

// Tools: (for now)

const unsigned char SIMULTY_CLIENT_TOOL_NONE           = 0;
const unsigned char SIMULTY_CLIENT_TOOL_LAND           = 1;
const unsigned char SIMULTY_CLIENT_TOOL_ROAD           = 2;

const unsigned char SIMULTY_CLIENT_TOOL_ZONE_RES       = 10;
const unsigned char SIMULTY_CLIENT_TOOL_ZONE_COM       = 11;
const unsigned char SIMULTY_CLIENT_TOOL_ZONE_IND       = 12;

const unsigned char SIMULTY_CLIENT_TOOL_BUILD_POLICE   = 50;
const unsigned char SIMULTY_CLIENT_TOOL_BUILD_HOSPITAL = 51;
const unsigned char SIMULTY_CLIENT_TOOL_BUILD_FIRE     = 52;

// Costs: (for now)

const unsigned NLINT16 SIMULTY_COST_ROAD = 10;
const unsigned NLINT16 SIMULTY_COST_LAND = 10;

const unsigned NLINT16 SIMULTY_COST_IND = 100;
const unsigned NLINT16 SIMULTY_COST_COM = 120;
const unsigned NLINT16 SIMULTY_COST_RES = 80;

const unsigned NLINT16 SIMULTY_COST_POLICE = 3000;
const unsigned NLINT16 SIMULTY_COST_HOSPITAL = 2600;
const unsigned NLINT16 SIMULTY_COST_FIRE = 1800;

// Packets:

const unsigned char NPACKET_TYPE_SIMULTY_WELCOME          = 100;
const unsigned char NPACKET_TYPE_SIMULTY_VERSION_CLIENT   = 101;
const unsigned char NPACKET_TYPE_SIMULTY_VERSION_SERVER   = 102;

const unsigned char NPACKET_TYPE_SIMULTY_ID               = 103;
const unsigned char NPACKET_TYPE_SIMULTY_GAMEDATA         = 104;

const unsigned char NPACKET_TYPE_SIMULTY_PLAYER_JOINED    = 120;
const unsigned char NPACKET_TYPE_SIMULTY_PLAYER_LEFT      = 121;


const unsigned char NPACKET_TYPE_SIMULTY_ROAD_BUILD       = 200;
const unsigned char NPACKET_TYPE_SIMULTY_ROAD_DESTROY     = 201;

const unsigned char NPACKET_TYPE_SIMULTY_BUILDING_BUILD   = 204;
const unsigned char NPACKET_TYPE_SIMULTY_BUILDING_DESTROY = 205;

const unsigned char NPACKET_TYPE_SIMULTY_LAND_BUY  = 230;
const unsigned char NPACKET_TYPE_SIMULTY_LAND_SELL = 231;

const unsigned char NPACKET_TYPE_SIMULTY_LAND_ZONE   = 232;
const unsigned char NPACKET_TYPE_SIMULTY_LAND_DEZONE = 233;


const unsigned char NPACKET_TYPE_SIMULTY_TIME_INCR  = 210;
const unsigned char NPACKET_TYPE_SIMULTY_TIME_SPEED = 211;

const unsigned char NPACKET_TYPE_SIMULTY_MONEY_CHANGE = 130;



#endif

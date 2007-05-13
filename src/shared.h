#ifndef _SHARED_H_
#define _SHARED_H_



#define err cerr

const unsigned char SIMULTY_MAX_PLAYERS = 16;

// Directions:

const unsigned char DIR_NONE  = 0;
const unsigned char DIR_UP    = 1;
const unsigned char DIR_RIGHT = 2;
const unsigned char DIR_DOWN  = 3;
const unsigned char DIR_LEFT  = 4;


// Costs: (for now)

const unsigned char SIMULTY_COST_ROAD = 10;
const unsigned char SIMULTY_COST_LAND = 10;

const unsigned char SIMULTY_COST_ZONE_IND = 100;
const unsigned char SIMULTY_COST_ZONE_COM = 120;
const unsigned char SIMULTY_COST_ZONE_RES = 80;

// Packets:

const unsigned char NPACKET_TYPE_SIMULTY_WELCOME = 100;
const unsigned char NPACKET_TYPE_SIMULTY_VERSION_CLIENT = 101;
const unsigned char NPACKET_TYPE_SIMULTY_VERSION_SERVER = 102;

const unsigned char NPACKET_TYPE_SIMULTY_ID      = 103;

const unsigned char NPACKET_TYPE_SIMULTY_PLAYER_JOINED  = 120;
const unsigned char NPACKET_TYPE_SIMULTY_PLAYER_LEFT = 121;


const unsigned char NPACKET_TYPE_SIMULTY_ROAD_BUILD = 200;
const unsigned char NPACKET_TYPE_SIMULTY_ROAD_DESTROY = 201;

const unsigned char NPACKET_TYPE_SIMULTY_LAND_BUY  = 230;
const unsigned char NPACKET_TYPE_SIMULTY_LAND_SELL = 231;

const unsigned char NPACKET_TYPE_SIMULTY_LAND_ZONE   = 232;
const unsigned char NPACKET_TYPE_SIMULTY_LAND_DEZONE = 233;


const unsigned char NPACKET_TYPE_SIMULTY_TIME_INCR  = 210;
const unsigned char NPACKET_TYPE_SIMULTY_TIME_SPEED = 211;

const unsigned char NPACKET_TYPE_SIMULTY_MONEY_CHANGE = 130;



#endif

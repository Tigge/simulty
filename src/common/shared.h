#ifndef _SHARED_H_
#define _SHARED_H_

#include "NL.hpp"


//#define err std::cerr

const unsigned char SIMULTY_MAX_PLAYERS = 16;

// Directions:

const unsigned char DIR_UP    = 0;
const unsigned char DIR_RIGHT = 1;
const unsigned char DIR_DOWN  = 2;
const unsigned char DIR_LEFT  = 3;

const unsigned char DIR_NONE  = 4;

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

const unsigned char SIMULTY_CLIENT_TOOL_BULLDOZER      = 100;
const unsigned char SIMULTY_CLIENT_TOOL_DEZONE         = 101;

// Costs: (for now)

const unsigned NLINT16 SIMULTY_COST_ROAD     = 10;
const unsigned NLINT16 SIMULTY_COST_LAND     = 10;

const unsigned NLINT16 SIMULTY_COST_IND      = 100;
const unsigned NLINT16 SIMULTY_COST_COM      = 120;
const unsigned NLINT16 SIMULTY_COST_RES      = 80;

const unsigned NLINT16 SIMULTY_COST_POLICE   = 3000;
const unsigned NLINT16 SIMULTY_COST_HOSPITAL = 2600;
const unsigned NLINT16 SIMULTY_COST_FIRE     = 1800;

// Packets:
// =============================================================================

const unsigned char NLPACKET_TYPE_SIMULTY_WELCOME                  = 100;
const unsigned char NLPACKET_TYPE_SIMULTY_VERSION_CLIENT           = 101;
const unsigned char NLPACKET_TYPE_SIMULTY_VERSION_SERVER           = 102;

const unsigned char NLPACKET_TYPE_SIMULTY_ID                       = 103;
const unsigned char NLPACKET_TYPE_SIMULTY_GAMEDATA                 = 104;

const unsigned char NLPACKET_TYPE_SIMULTY_PLAYER_JOINED            = 120;
const unsigned char NLPACKET_TYPE_SIMULTY_PLAYER_LEFT              = 121;

// Requests:

const unsigned char NLPACKET_TYPE_SIMULTY_REQUEST_ROAD             = 150;
const unsigned char NLPACKET_TYPE_SIMULTY_REQUEST_ZONE             = 151;
const unsigned char NLPACKET_TYPE_SIMULTY_REQUEST_LAND             = 152;
const unsigned char NLPACKET_TYPE_SIMULTY_REQUEST_CHAT             = 153;
const unsigned char NLPACKET_TYPE_SIMULTY_REQUEST_SPECIAL_BUILDING = 154;
const unsigned char NLPACKET_TYPE_SIMULTY_REQUEST_BULLDOZE         = 160;
const unsigned char NLPACKET_TYPE_SIMULTY_REQUEST_DEZONE           = 161;

const unsigned char NLPACKET_TYPE_SIMULTY_REQUEST_DEBUG            = 255;

// Responese:

const unsigned char NLPACKET_TYPE_SIMULTY_ROAD                     = 200;
const unsigned char NLPACKET_TYPE_SIMULTY_ZONE                     = 201;
const unsigned char NLPACKET_TYPE_SIMULTY_LAND                     = 202;
const unsigned char NLPACKET_TYPE_SIMULTY_CHAT                     = 203;
const unsigned char NLPACKET_TYPE_SIMULTY_BUILD_SPECIAL_BUILDING   = 211;
const unsigned char NLPACKET_TYPE_SIMULTY_REMOVE_SPECIAL_BUILDING  = 212;
const unsigned char NLPACKET_TYPE_SIMULTY_BUILD_ZONE_BUILDING      = 213;
const unsigned char NLPACKET_TYPE_SIMULTY_REMOVE_ZONE_BUILDING     = 214;
const unsigned char NLPACKET_TYPE_SIMULTY_BULLDOZE                 = 220;
const unsigned char NLPACKET_TYPE_SIMULTY_DEZONE                   = 221;

const unsigned char NLPACKET_TYPE_SIMULTY_THRIVEMAP                = 225;

// Other:

const unsigned char NLPACKET_TYPE_SIMULTY_TIME_INCR               = 230;
const unsigned char NLPACKET_TYPE_SIMULTY_TIME_SPEED              = 231;

const unsigned char NLPACKET_TYPE_SIMULTY_BUDGET_UPDATE           = 132;


// Events:

const int           USEREVENT_TIMEINCREMENT                       = 100;
const int           USEREVENT_GOTPACKET                           = 200;
const int           USEREVENT_GOTPLAYER                           = 300;
const int           USEREVENT_LOSTPLAYER                          = 301;

#endif

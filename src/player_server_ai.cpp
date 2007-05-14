#include "player_server_ai.h"

player_server_ai::player_server_ai (NL_INT32 id_new, NL_INT16 slot_new) : player(id_new, slot_new) {

    type = PLAYER_TYPE_SERVER_AI;
}

player_server_ai::~player_server_ai (  ){
}

void player_server_ai::update (  ){
}


#include "player_client_remote.h"

player_client_remote::player_client_remote(INT32 id_new, INT16 slot_new) : player(id_new, slot_new)
{

    type = PLAYER_TYPE_CLIENT_REMOTE;

}

player_client_remote::~player_client_remote (  ){
}

void player_client_remote::update (  ){
}


#include "player_client_remote.h"

player_client_remote::player_client_remote(NLINT32 id_new, NLINT16 slot_new) : Player(id_new, slot_new)
{

    type = PLAYER_TYPE_CLIENT_REMOTE;

}

player_client_remote::~player_client_remote (  ){
}

void player_client_remote::update (  ){
}


#include "player_client_local.h"


player_client_local::player_client_local(NL_INT32 id_new, NL_INT16 slot_new) : player(id_new, slot_new)
{
   type = PLAYER_TYPE_CLIENT_LOCAL;
}
/*
player_client_local::player_client_local (  ){

    type = PLAYER_TYPE_LOCAL;
}
*/

player_client_local::~player_client_local()
{

}

void player_client_local::update ()
{

}


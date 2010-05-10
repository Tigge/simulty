#include "PlayerClientLocal.hpp"


PlayerClientLocal::PlayerClientLocal(NLINT32 id_new, NLINT16 slot_new): Player(id_new, slot_new)
{
   type = PLAYER_TYPE_CLIENT_LOCAL;
}


PlayerClientLocal::~PlayerClientLocal() {

}

void PlayerClientLocal::update () {

}


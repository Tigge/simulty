#include "PlayerClientRemote.hpp"

PlayerClientRemote::PlayerClientRemote(NLINT32 id_new, NLINT16 slot_new): 
        Player(id_new, slot_new) {
    type = PLAYER_TYPE_CLIENT_REMOTE;
}

PlayerClientRemote::~PlayerClientRemote() {

}

void PlayerClientRemote::update() {

}


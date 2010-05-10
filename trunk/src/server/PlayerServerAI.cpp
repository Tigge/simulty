#include "PlayerServerAI.hpp"

PlayerServerAI::PlayerServerAI(NLINT32 idNew, NLINT16 slotNew) : Player(idNew, slotNew) {

    type = PLAYER_TYPE_SERVER_AI;
}

PlayerServerAI::~PlayerServerAI() {
}

void PlayerServerAI::update () {
}


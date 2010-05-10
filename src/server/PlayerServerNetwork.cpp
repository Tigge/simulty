#include "PlayerServerNetwork.hpp"

PlayerServerNetwork::PlayerServerNetwork(NLINT32 idNew, NLINT16 slotNew, 
        NL::Socket *socket) : Player(idNew, slotNew) {

    this->type     = PLAYER_TYPE_SERVER_NETWORK;
    this->socket   = socket;    
    this->sender   = new PacketSender(socket);
    this->receiver = new PacketReceiver(socket, USEREVENT_GOTPACKET, this);
    
    this->sender->start();
    this->receiver->start();
}

PlayerServerNetwork::~PlayerServerNetwork() {

}
/*
void player_server_network::setMoney(int money_new)
{
    // Update money locally:
    money = money_new;

    // Update remote money:
    NLPacket money_update(NLPACKET_TYPE_SIMULTY_MONEY_CHANGE);
    money_update << getId() << money_new;
    socket->packet_put(money_update);


}*/

void PlayerServerNetwork::update() {
}




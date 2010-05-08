#include "player_server_network.h"

player_server_network::player_server_network (NLINT32 id_new, NLINT16 slot_new) : Player(id_new, slot_new) {

    type = PLAYER_TYPE_SERVER_NETWORK;
}

player_server_network::~player_server_network()
{

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

void player_server_network::update (  ){
}




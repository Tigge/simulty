#include "player_server_network.h"

player_server_network::player_server_network (NL_INT32 id_new, NL_INT16 slot_new) : player(id_new, slot_new)
{
    type = PLAYER_TYPE_SERVER_NETWORK;
}

player_server_network::~player_server_network()
{

}

void player_server_network::money_set(int money_new)
{
    // Update money locally:
    money = money_new;

    // Update remote money:
    NPacket money_update(NPACKET_TYPE_SIMULTY_MONEY_CHANGE);
    money_update << id_get() << money_new;
    socket->packet_put(money_update);
    
    
}

void player_server_network::update (  ){
}




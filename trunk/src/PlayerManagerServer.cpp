#include "Server.hpp"
#include "NL.hpp"

#include "PlayerManagerServer.hpp"

PlayerManagerServer::PlayerManagerServer() : PlayerManager()
{


}

PlayerManagerServer::~PlayerManagerServer()
{


}



int  PlayerManagerServer::nextSlot()
{
    for(unsigned int slot = 0; slot < count_max(); slot++)
    {
        if(get_by_slot(slot) == NULL)
        {
            return slot;
        }
    }  
    
    return -1;

}

/*
void PlayerManagerServer::changeMoney(int slot, int money) {

  Player *p = get_by_slot(slot);
  
  if(p != NULL) {
    Server *server = Server::getInstance();
    p->setMoney(money);
    
    NLPacket packet(NLPACKET_TYPE_SIMULTY_MONEY_CHANGE);
    packet << (NLINT16)slot << (NLINT32)money;
    server->packet_send_to_all(packet);
  }

}
*/

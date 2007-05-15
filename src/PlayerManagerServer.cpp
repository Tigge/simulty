#include "PlayerManagerServer.hpp"

PlayerManagerServer::PlayerManagerServer() : PlayerManager()
{


}

PlayerManagerServer::~PlayerManagerServer()
{


}



int  PlayerManagerServer::slot_next()
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

#include "player_manager_server.h"

player_manager_server::player_manager_server() : player_manager()
{


}

player_manager_server::~player_manager_server()
{


}



int  player_manager_server::slot_next()
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

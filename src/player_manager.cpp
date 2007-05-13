#include "player_manager.h"


player_manager::player_manager()
{
    // Set up all slots:
    slots.insert(slots.begin(), SIMULTY_MAX_PLAYERS, 0);
    
    // Reserve list space
    list.reserve(SIMULTY_MAX_PLAYERS);
}

player_manager::~player_manager()
{



}
player *player_manager::get_by_n(unsigned int n)
{
    if(n >= 0 && n < count())return list[n];
    return NULL;
}

    
player *player_manager::get_by_id(INT32 i)
{
    for(unsigned int l = 0; l < count(); l++)
        if(list[l]->id_get() == i)return list[l];
        
    return NULL;
}

player *player_manager::get_by_slot(INT16 s)
{
    for(unsigned int l = 0; l < count(); l++)
        if(list[l]->slot_get() == s)return list[l];
        
    return NULL;
}

bool player_manager::add(player *p)
{
    if(count() == count_max()) return false;    
    
    list.push_back(p);
    return true;    
}


bool player_manager::del_by_p(player* p)
{
    // Fidn, todo, use STL
    int n = -1;
    for(unsigned int i = 0; i < count(); i++)
    {
        if(list[i] == p) { n = i; break; }
    }

    if(n == -1) return false;
    

    err << "* Removing player" << endl;

    // Empty slot:
    slots[p->slot_get()] = 0;

    // Delete memory and run deconstructor:
    if(p->type_get() == PLAYER_TYPE_SERVER_NETWORK)
    {
        player_server_network *play = (player_server_network *)p;
        err << "  - Was connected from " << play->socket->get_ip() << ":" << play->socket->get_port() << endl;

        delete (player_server_network *)p;
    }
    else if(p->type_get() == PLAYER_TYPE_SERVER_AI)
    {
        
        delete (player_server_ai *)p;
        err << "  - Was an AI that runned on the server" << endl;
    }

    err << endl;

    // Erase from list:
    list.erase(list.begin() + n);

    return true;
}

bool player_manager::del_by_id(INT32 i)
{
    for(unsigned int l = 0; l < count(); l++)
        if(list[l]->id_get() == i)return del_by_p(list[l]);
        
    return false;
}

bool player_manager::del_by_slot(INT16 s)
{
    for(unsigned int l = 0; l < count(); l++)
        if(list[l]->slot_get() == s)return del_by_p(list[l]);
        
    return false;    
}


unsigned int  player_manager::count()
{
    return list.size();

}
unsigned int  player_manager::count_max()
{
    return SIMULTY_MAX_PLAYERS;
}
    
void player_manager::update()
{
    for(unsigned int i = 0; i < count(); i++)
    {
        list[i]->update();
    }
}


bool player_manager::full()
{
    return count() == count_max();
}



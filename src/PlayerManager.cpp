#include "PlayerManager.hpp"


PlayerManager::PlayerManager()
{
    // Set up all slots:
    slots.insert(slots.begin(), SIMULTY_MAX_PLAYERS, 0);
    
    // Reserve list space
    list.reserve(SIMULTY_MAX_PLAYERS);
}

PlayerManager::~PlayerManager()
{



}
Player *PlayerManager::get_by_n(unsigned int n)
{
    if(n >= 0 && n < count())return list[n];
    return NULL;
}

    
Player *PlayerManager::get_by_id(NLINT32 i)
{
    for(unsigned int l = 0; l < count(); l++)
        if(list[l]->id_get() == i)return list[l];
        
    return NULL;
}

Player *PlayerManager::get_by_slot(NLINT16 s)
{
    for(unsigned int l = 0; l < count(); l++)
        if(list[l]->slot_get() == s)return list[l];
        
    return NULL;
}

bool PlayerManager::add(Player *p)
{
    if(count() == count_max()) return false;    
    
    list.push_back(p);
    return true;    
}


bool PlayerManager::del_by_p(Player* p)
{
    // Fidn, todo, use STL
    int n = -1;
    for(unsigned int i = 0; i < count(); i++)
    {
        if(list[i] == p) { n = i; break; }
    }

    if(n == -1) return false;
    

    std::cerr << "* Removing Player" << std::endl;

    // Empty slot:
    slots[p->slot_get()] = 0;

    // Delete memory and run deconstructor:
    if(p->type_get() == PLAYER_TYPE_SERVER_NETWORK)
    {
        player_server_network *play = (player_server_network *)p;
        std::cerr << "  - Was connected from " << play->socket->get_ip() << ":" << play->socket->get_port() << std::endl;

        delete (player_server_network *)p;
    }
    else if(p->type_get() == PLAYER_TYPE_SERVER_AI)
    {
        
        delete (player_server_ai *)p;
        std::cerr << "  - Was an AI that runned on the server" << std::endl;
    }

    std::cerr << std::endl;

    // Erase from list:
    list.erase(list.begin() + n);

    return true;
}

bool PlayerManager::del_by_id(NLINT32 i)
{
    for(unsigned int l = 0; l < count(); l++)
        if(list[l]->id_get() == i)return del_by_p(list[l]);
        
    return false;
}

bool PlayerManager::del_by_slot(NLINT16 s)
{
    for(unsigned int l = 0; l < count(); l++)
        if(list[l]->slot_get() == s)return del_by_p(list[l]);
        
    return false;    
}


unsigned int  PlayerManager::count()
{
    return list.size();

}
unsigned int  PlayerManager::count_max()
{
    return SIMULTY_MAX_PLAYERS;
}
    
void PlayerManager::update()
{
    for(unsigned int i = 0; i < count(); i++)
    {
        list[i]->update();
    }
}


bool PlayerManager::full()
{
    return count() == count_max();
}



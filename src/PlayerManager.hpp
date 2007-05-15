#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "shared.h"

#include "NL.hpp"

#include "Player.hpp"
#include "player_client_local.h"
#include "player_client_remote.h"
#include "player_server_network.h"
#include "player_server_ai.h"

class PlayerManager
{
    protected:
    
    std::vector<Player *> list;
    std::vector<int>      slots;
    
    public:
    
    PlayerManager();
    virtual ~PlayerManager();
    
    virtual Player *get_by_n(unsigned int n);
    virtual Player *get_by_id(NLINT32 i);
    virtual Player *get_by_slot(NLINT16 s);

    virtual bool add(Player *p);

    virtual bool del_by_p(Player* p);
    virtual bool del_by_id(NLINT32 i);
    virtual bool del_by_slot(NLINT16 s);

    unsigned int count();
    unsigned int count_max();
    
    bool         full();
    
    virtual void update();
    
};

#endif

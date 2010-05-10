#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "shared.h"

#include "NL.hpp"

#include "Player.hpp"
#include "../client/PlayerClientLocal.hpp"
#include "../client/PlayerClientRemote.hpp"
#include "../server/PlayerServerNetwork.hpp"
#include "../server/PlayerServerAI.hpp"

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

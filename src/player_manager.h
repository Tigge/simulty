#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "shared.h"

#include "netlib.h"

#include "player.h"
#include "player_client_local.h"
#include "player_client_remote.h"
#include "player_server_network.h"
#include "player_server_ai.h"

class player_manager
{
    protected:
    
    std::vector<player *> list;
    std::vector<int>      slots;
    
    public:
    
    player_manager();
    virtual ~player_manager();
    
    virtual player *get_by_n(unsigned int n);
    virtual player *get_by_id(INT32 i);
    virtual player *get_by_slot(INT16 s);

    virtual bool add(player *p);

    virtual bool del_by_p(player* p);
    virtual bool del_by_id(INT32 i);
    virtual bool del_by_slot(INT16 s);

    unsigned int count();
    unsigned int count_max();
    bool         full();
    
    virtual void update();
    
};

#endif

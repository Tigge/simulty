#include "player.h"


player::player(NL_INT32 id_new, NL_INT16 slot_new)
{
    id    = id_new;
    slot  = slot_new;
    type  = PLAYER_TYPE_BASE;
    money = 20000;
}

/*
player::player() : player(-1, -1)
{
    
    type = PLAYER_TYPE_BASE;
    money = 20000;
}
*/

player::~player()
{

}

void player::money_set(int m)
{
    money = m;
}


int  player::money_get()
{
    return money;
}



void player::update()
{

}


int  player::id_get() 
{
    return id;
}

int  player::slot_get()
{
    return slot;
}

int player::type_get()
{
    return type;
}

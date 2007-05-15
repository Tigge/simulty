#include "Player.h"


Player::Player(NL_INT32 id_new, NL_INT16 slot_new)
{
    id    = id_new;
    slot  = slot_new;
    type  = PLAYER_TYPE_BASE;
    money = 20000;
}

/*
Player::Player() : Player(-1, -1)
{
    
    type = Player_TYPE_BASE;
    money = 20000;
}
*/

Player::~Player()
{

}

void Player::money_set(int m)
{
    money = m;
}


int  Player::money_get()
{
    return money;
}



void Player::update()
{

}


int  Player::id_get() 
{
    return id;
}

int  Player::slot_get()
{
    return slot;
}

int Player::type_get()
{
    return type;
}

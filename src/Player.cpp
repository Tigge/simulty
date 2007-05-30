#include "Player.hpp"


Player::Player(NLINT32 id_new, NLINT16 slot_new)
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

void Player::setMoney(int m)
{
    money = m;
}


int  Player::getMoney()
{
    return money;
}



void Player::update()
{

}


int  Player::getId()
{
    return id;
}

int  Player::getSlot()
{
    return slot;
}

int Player::getType()
{
    return type;
}

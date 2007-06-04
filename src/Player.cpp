#include "Player.hpp"


Player::Player(NLINT32 id_new, NLINT16 slot_new) {

    id    = id_new;
    slot  = slot_new;
    type  = PLAYER_TYPE_BASE;
    money = 20000;

    // Taxes in percantage
    tax = 10;
    expences_year = expences_last = level_electricity
    = level_police = level_fire = level_education = 0;
}

/*
Player::Player() : Player(-1, -1)
{

    type = Player_TYPE_BASE;
    money = 20000;
}
*/

Player::~Player() {
}

int  Player::getMoney() {
  return money;
}
void Player::setMoney(int m) {
  money = m;
}

int  Player::getTax() {
  return tax;
}
void Player::setTax(unsigned int tax) {
  this->tax = tax;
}

void Player::update() {
}


int  Player::getId() {
    return id;
}

int  Player::getSlot() {
    return slot;
}

int Player::getType() {
    return type;
}

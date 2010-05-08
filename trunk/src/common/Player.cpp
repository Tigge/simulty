#include "Player.hpp"


Player::Player(NLINT32 id_new, NLINT16 slot_new) {

    // Set ID slot and type
    id     = id_new;
    slot   = slot_new;
    type   = PLAYER_TYPE_BASE;
    
    // Assign starting money and tax
    budget = new Budget(20000);
    budget->setTax(0.10);
    
}

Player::~Player() {
}

void Player::setThriveMap(ThriveMap *thriveMap) {
  this->thriveMap = thriveMap;
}
ThriveMap *Player::getThriveMap() {
  return this->thriveMap;
}

void Player::setBudget(Budget *budget) {
  this->budget = budget;
}
Budget *Player::getBudget() {
  return this->budget;
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

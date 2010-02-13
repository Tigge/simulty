#include "StatusBar.hpp"


StatusBar::StatusBar(Client *client) {
  this->client = client;
  
  setHeight(20);
  setOpaque(true);
  
  this->moneyLabel = new gcn::Label("Money");
  add(moneyLabel, 3, 3);
  
  this->timeLabel = new gcn::Label("Money");
  add(timeLabel, 100, 3);
  
}

void StatusBar::logic() {
  Container::logic();
  
  if(client->state_game == SIMULTY_CLIENT_STATE_GAME_ON) {
  
    //std::cout << "'" << (int)client->getMyPlayer()->getBudget()->getBalance() << "'" << std::endl;
    char moneyText[255];
    sprintf(moneyText, "Money: %d", client->getMyPlayer()->getBudget()->getBalance());
    moneyLabel->setCaption(moneyText);
    moneyLabel->adjustSize();
    
    char timeText[255];
    sprintf(timeText, "Time: %i %s %i", client->date.getYear(), client->date.getMonthAsString().c_str(), client->date.getDay());
    timeLabel->setCaption(timeText);
    timeLabel->adjustSize();
  }
}


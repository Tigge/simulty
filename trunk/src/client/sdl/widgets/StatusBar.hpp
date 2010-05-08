#ifndef _STATUSBAR_HPP_
#define _STATUSBAR_HPP_

#include "guichan.hpp"

#include "../../Client.hpp"

class StatusBar : public gcn::Container {

  private:
  
  Client     *client;
  
  gcn::Label *moneyLabel;
  gcn::Label *timeLabel;
  
  public:
  
  StatusBar(Client *client);
  virtual void logic();
};

#endif

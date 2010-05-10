#include "MainMenu.hpp"

#include <iostream>
#include <sstream>

/*
#include <libintl.h>
#include <gettext.h>
#define _(string) gettext (string)
*/
#define _(string) string

MainMenu::MainMenu(gcn::ActionListener *al) : Window("Main Menu") {

  setDimension(gcn::Rectangle(0, 0, 150, 200));
  setBaseColor(gcn::Color(255, 200, 200, 200));

  connectLabel = new gcn::Label("IP address");
  add(connectLabel, 10,  10);

  address = new gcn::TextField("192.168.0.1");
  address->setWidth(120);
  add(address, 10, 30);

  connect = new gcn::Button("Connect");
  connect->addActionListener(al);
  connect->setActionEventId("mainmenu-connect");
  connect->setWidth(120);
  add(connect, 10, 60);
  
  exit = new gcn::Button("Exit");
  exit->addActionListener(al);
  exit->setActionEventId("mainmenu-exit");
  add(exit, 10, 100);

}


MainMenu::~MainMenu() {

  delete connectLabel;
  delete address;
  delete connect;
  
  delete exit;

}


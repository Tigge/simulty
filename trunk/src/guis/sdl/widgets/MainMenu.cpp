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

  label = new gcn::Label();
  label->setCaption(_("Connect or Exit?"));
  add(label, 10,  10);

  connect = new gcn::Button("Connect");
  connect->addActionListener(al);
  connect->setActionEventId("mainmenu-connect");
  add(connect, 10, 30);
  
  exit = new gcn::Button("Exit");
  exit->addActionListener(al);
  exit->setActionEventId("mainmenu-exit");
  add(exit, 10, 80);

}


MainMenu::~MainMenu() {

  delete label;
  
  delete connect;
  delete exit;

}


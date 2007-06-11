#include "Toolbar.hpp"

#include "../../../shared.h"
#include <iostream>
Toolbar::Toolbar() : gcn::Container() {

  setWidth(64);
  setHeight(160);

  gcn::ImageLoader *imageLoader = gcn::Image::getImageLoader();

  indIcon   = imageLoader->load("img/menu_icon_ind.pcx", true);
  indButton = new gcn::ImageButton(indIcon);
  indButton->addActionListener(this);
  add(indButton, 0, 0);

  comIcon   = imageLoader->load("img/menu_icon_com.pcx", true);
  comButton = new gcn::ImageButton(comIcon);
  comButton->addActionListener(this);
  add(comButton, 32, 0);

  resIcon   = imageLoader->load("img/menu_icon_res.pcx", true);
  resButton = new gcn::ImageButton(resIcon);
  resButton->addActionListener(this);
  add(resButton, 0, 32);

  roadIcon  = imageLoader->load("img/menu_icon_road.pcx", true);
  roadButton = new gcn::ImageButton(roadIcon);
  roadButton->addActionListener(this);
  add(roadButton, 32, 32);

  landIcon  = imageLoader->load("img/menu_icon_land.pcx", true);
  landButton = new gcn::ImageButton(landIcon);
  landButton->addActionListener(this);
  add(landButton, 0, 64);


  policeIcon   = imageLoader->load("img/menu_icon_police.pcx", true);
  policeButton = new gcn::ImageButton(policeIcon);
  policeButton->addActionListener(this);
  add(policeButton, 32, 64);

  fireIcon  = imageLoader->load("img/menu_icon_fire.pcx", true);
  fireButton = new gcn::ImageButton(fireIcon);
  fireButton->addActionListener(this);
  add(fireButton, 0, 96);


  hospitalIcon  = imageLoader->load("img/menu_icon_hospital.pcx", true);
  hospitalButton = new gcn::ImageButton(hospitalIcon);
  hospitalButton->addActionListener(this);
  add(hospitalButton, 32, 96);

  bulldozerIcon  = imageLoader->load("img/menu_icon_bulldozer.pcx", true);
  bulldozerButton = new gcn::ImageButton(bulldozerIcon);
  bulldozerButton->addActionListener(this);
  add(bulldozerButton, 0, 128);

  tool = SIMULTY_CLIENT_TOOL_LAND;

  std::cout << "toolbar is inited" << std::endl;

}
Toolbar::~Toolbar() {

  delete roadIcon;
  delete roadButton;

  delete landIcon;
  delete landButton;

  delete policeIcon;
  delete policeButton;

  delete fireIcon;
  delete fireButton;

  delete hospitalIcon;
  delete hospitalButton;

  delete bulldozerIcon;
  delete bulldozerButton;

  delete resButton;
  delete comButton;
  delete indButton;

  delete resIcon;
  delete comIcon;
  delete indIcon;

}
  
void Toolbar::action(const gcn::ActionEvent &e) {

  if(e.getSource() == comButton) {
    tool = SIMULTY_CLIENT_TOOL_ZONE_COM;
  } else if(e.getSource() == resButton) {
    tool = SIMULTY_CLIENT_TOOL_ZONE_RES;
  } else if(e.getSource() == indButton) {
    tool = SIMULTY_CLIENT_TOOL_ZONE_IND;
  } else if(e.getSource() == roadButton) {
    tool = SIMULTY_CLIENT_TOOL_ROAD;
  } else if(e.getSource() == landButton) {
    tool = SIMULTY_CLIENT_TOOL_LAND;
  } else if(e.getSource() == policeButton) {
    tool = SIMULTY_CLIENT_TOOL_BUILD_POLICE;
  } else if(e.getSource() == fireButton) {
    tool = SIMULTY_CLIENT_TOOL_BUILD_FIRE;
  } else if(e.getSource() == hospitalButton) {
    tool = SIMULTY_CLIENT_TOOL_BUILD_HOSPITAL;
  } else if(e.getSource() == bulldozerButton) {
    tool = SIMULTY_CLIENT_TOOL_BULLDOZER;
  }

}

int Toolbar::getTool() {
  return this->tool;
}

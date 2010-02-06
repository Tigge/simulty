#ifndef _MAINMENU_HPP_
#define _MAINMENU_HPP_

#include <guichan.hpp>
#include "../SDLGUI.hpp"

class MainMenu : public gcn::Window {

  private:
  
  gcn::Label  *label;
  
  gcn::Button *connect;
  gcn::Button *exit;

  public:
  
  MainMenu(gcn::ActionListener *al);
  virtual ~MainMenu();

};

#endif

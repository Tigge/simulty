
#ifndef _TOOLBAR_HPP_
#define _TOOLBAR_HPP_

#include "guichan.hpp"
#include "shared.h"

//#include "ImageButton.hpp"

class Toolbar : public gcn::Container, gcn::ActionListener {


  private:

  gcn::Button     *indButton;
  gcn::Button     *resButton;
  gcn::Button     *comButton;

  gcn::Button      *roadButton;
  gcn::Button      *landButton;
  
  gcn::Button      *policeButton;
  gcn::Button      *fireButton;
  gcn::Button      *hospitalButton;
  gcn::Button      *powerplantButton;
  
  gcn::Button      *bulldozerButton;
  gcn::Button      *deZoneButton;

  gcn::Image       *indIcon;
  gcn::Image       *resIcon;
  gcn::Image       *comIcon;

  gcn::Image       *roadIcon;
  gcn::Image       *landIcon;
  
  gcn::Image       *policeIcon;
  gcn::Image       *fireIcon;
  gcn::Image       *hospitalIcon;
  gcn::Image       *powerplantIcon;
  gcn::Image       *bulldozerIcon;
  gcn::Image       *deZoneIcon;

  int tool;

  public:

  Toolbar();
  virtual ~Toolbar();
  
  void action(const gcn::ActionEvent &e);

  int getTool();

};


#endif


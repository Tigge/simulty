
#ifndef _TOOLBAR_HPP_
#define _TOOLBAR_HPP_

#include "allegro.h"
#ifdef WIN32
  #include "winalleg.h"
#endif
#include <guichan.hpp>
#include <guichan/allegro.hpp>

#include "ImageButton.hpp"

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
  
  gcn::Button      *bulldozerButton;

  gcn::Image       *indIcon;
  gcn::Image       *resIcon;
  gcn::Image       *comIcon;

  gcn::Image       *roadIcon;
  gcn::Image       *landIcon;
  
  gcn::Image       *policeIcon;
  gcn::Image       *fireIcon;
  gcn::Image       *hospitalIcon;
  gcn::Image       *bulldozerIcon;

  int tool;

  public:

  Toolbar();
  virtual ~Toolbar();
  
  void action(const gcn::ActionEvent &e);

  int getTool();

};


#endif


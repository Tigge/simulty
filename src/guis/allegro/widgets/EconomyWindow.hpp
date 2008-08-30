#ifndef _ECONOMYWINDOW_HPP_
#define _ECONOMYWINDOW_HPP_

#include <guichan.hpp>
#include "../../../Budget.hpp"

class EconomyWindow : public gcn::Window, gcn::ActionListener {

  private:
  
  gcn::Label  **labels;
  gcn::Label  **values;

  gcn::Label  *taxLabel;
  gcn::Slider *taxSlider;
  
  gcn::Button *cancelButton;
  gcn::Button *okButton;

  public:
  
  EconomyWindow();
  virtual ~EconomyWindow();
  
  void update(Budget *);
  void action(const gcn::ActionEvent &e);

};

#endif

#ifndef _ECONOMYWINDOW_HPP_
#define _ECONOMYWINDOW_HPP_

#include <guichan.hpp>

class EconomyWindow : public gcn::Window, gcn::ActionListener {

  private:

  gcn::Label  *taxLabel;
  gcn::Slider *taxSlider;

  gcn::Label  *incomeLabel;
  gcn::Label  *incomeValue;
  
  gcn::Label  *expenceLabel;
  gcn::Label  *expenceValue;
  
  gcn::Button *cancelButton;
  gcn::Button *okButton;

  public:
  
  EconomyWindow(const std::string &caption);
  virtual ~EconomyWindow();
  
  void action(const gcn::ActionEvent &e);

};

#endif

#include "EconomyWindow.hpp"

#include <iostream>

/*
#include <libintl.h>
#include <gettext.h>
#define _(string) gettext (string)
*/
#define _(string) string

EconomyWindow::EconomyWindow(const std::string &caption) : Window(caption) {

  setDimension(gcn::Rectangle(0, 0, 300, 200));
  setBaseColor(gcn::Color(255, 150, 200, 190));

  taxLabel = new gcn::Label("Tax");
  add(taxLabel, 10, 10);

  taxSlider = new gcn::Slider(0.0, 100.0);
  taxSlider->setSize(100, 10);
  taxSlider->addActionListener(this);
  add(taxSlider, 100, 10);

  incomeLabel = new gcn::Label("Income");
  add(incomeLabel, 10, 30);
  
  incomeValue = new gcn::Label("0");
  add(incomeValue, 100, 30);
  
  expenceLabel = new gcn::Label("Expences");
  add(expenceLabel, 10, 50);
  
  expenceValue = new gcn::Label("0");
  add(expenceValue, 100, 50);
  
  cancelButton = new gcn::Button("Cancel");
  cancelButton->addActionListener(this);
  add(cancelButton, 10, 70);
  
  okButton = new gcn::Button("Ok");
  okButton->addActionListener(this);
  add(okButton, 100, 70);

}


EconomyWindow::~EconomyWindow() {

  delete taxLabel;
  delete taxSlider;

  delete incomeLabel;
  delete incomeValue;
  
  delete expenceLabel;
  delete expenceValue;
  
  delete cancelButton;
  delete okButton;

}


void EconomyWindow::action(const gcn::ActionEvent &e) {

  std::cout << "Action happend" << std::endl;

  if(e.getSource() == cancelButton) {
    setVisible(false);
  } else if(e.getSource() == okButton) {
    std::cout << "TODO: Update tax here" << std::endl;
  }

}

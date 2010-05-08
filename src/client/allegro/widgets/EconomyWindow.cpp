#include "EconomyWindow.hpp"

#include <iostream>
#include <sstream>

/*
#include <libintl.h>
#include <gettext.h>
#define _(string) gettext (string)
*/
#define _(string) string

EconomyWindow::EconomyWindow() : Window("Budget") {

  int height = Budget::ITEM_COUNT * 15 + 100;
  int width  = 200;

  setDimension(gcn::Rectangle(0, 0, width, height));
  setBaseColor(gcn::Color(255, 200, 200, 200));

  labels = new gcn::Label*[Budget::ITEM_COUNT];
  values = new gcn::Label*[Budget::ITEM_COUNT];
  
  for(int i = 0; i < Budget::ITEM_COUNT; i++) {
    labels[i] = new gcn::Label();
    values[i] = new gcn::Label();
    values[i]->setAlignment(gcn::Graphics::RIGHT);
    add(labels[i], 10,  10 + 15 * i);
    add(values[i], 180, 10 + 15 * i);
  }

  labels[Budget::ITEM_INCOME_TAXES_RESIDENTAL]->setCaption(_("Residental"));
  labels[Budget::ITEM_INCOME_TAXES_COMMERSIAL]->setCaption(_("Residental"));
  labels[Budget::ITEM_INCOME_TAXES_INDUSTRIAL]->setCaption(_("Residental"));

  labels[Budget::ITEM_EXPENSE_TRAFFIC]->setCaption(_("Traffic"));

  labels[Budget::ITEM_EXPENSE_CRIME]->setCaption(_("Crime figting"));
  labels[Budget::ITEM_EXPENSE_FIRE]->setCaption(_("Fire figting"));
  labels[Budget::ITEM_EXPENSE_HEALTH]->setCaption(_("Public health"));

  labels[Budget::ITEM_EXPENSE_ELECTRICITY]->setCaption(_("Electricity"));
  labels[Budget::ITEM_EXPENSE_WATER]->setCaption(_("Water"));
  labels[Budget::ITEM_EXPENSE_ENVIRONMENT]->setCaption(_("Environment"));
  labels[Budget::ITEM_EXPENSE_CONNECTIVITY]->setCaption(_("Connectivity"));
  
  labels[Budget::ITEM_EXPENSE_BUILD]->setCaption(_("Building"));


  taxLabel = new gcn::Label("Tax");
  add(taxLabel, 10, height - 60);

  taxSlider = new gcn::Slider(0.0, 100.0);
  taxSlider->setSize(80, 10);
  taxSlider->addActionListener(this);
  add(taxSlider, 100, height - 60);
  
  
  cancelButton = new gcn::Button("Cancel");
  cancelButton->addActionListener(this);
  add(cancelButton, 10, height - 40);
  
  okButton = new gcn::Button("Ok");
  okButton->addActionListener(this);
  add(okButton, 100, height - 40);


}


EconomyWindow::~EconomyWindow() {

  for(int i = 0; i < Budget::ITEM_COUNT; i++) {
    delete labels[i];
    delete values[i];
  }
  
  delete[] labels;
  delete[] values;

  delete taxLabel;
  delete taxSlider;
  
  delete cancelButton;
  delete okButton;

}


void EconomyWindow::update(Budget *budget) {

  for(int i = 0; i < Budget::ITEM_COUNT; i++) {
    int balance = budget->getBudgetItem(i);
    std::stringstream ss;
    ss << balance;  
    values[i]->setCaption(ss.str());  
  }

}

void EconomyWindow::action(const gcn::ActionEvent &e) {

  std::cout << "Action happend" << std::endl;

  if(e.getSource() == cancelButton) {
    setVisible(false);
  } else if(e.getSource() == okButton) {
    std::cout << "TODO: Update tax here" << std::endl;
  }

}

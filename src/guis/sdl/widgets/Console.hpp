#ifndef _CONSOLE_HPP_
#define _CONSOLE_HPP_

#include "guichan.hpp"

#include <vector>
#include <string>

class Console : public gcn::Container {

  private:
  
  static const int MAX_LINES = 5;
  
  std::vector<std::string> lines;
  gcn::Label             **lineLabels;

  bool showing;

  public:
  
  Console();
  
  void addLine(std::string line);
  void show();
  void hide();
  void toggle();
};

#endif

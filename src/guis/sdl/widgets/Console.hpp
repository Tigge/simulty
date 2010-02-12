#ifndef _CONSOLE_HPP_
#define _CONSOLE_HPP_

#include "guichan.hpp"

#include <vector>
#include <string>

class Console : public gcn::Widget {

  private:
  
  std::vector<std::string> lines;

  public:
  
  void addLine(std::string line);
  void show();
  void hide();
};

#endif

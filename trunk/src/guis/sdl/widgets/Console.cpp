#include "Console.hpp"

Console::Console() : lines(5) {
  setBackgroundColor(gcn::Color(0, 0, 0));
  setOpaque(true);
  
  // Create empty lines:
  lineLabels = new gcn::Label *[MAX_LINES];  
  for(int i = 0; i < MAX_LINES; i++) {
    lineLabels[i] = new gcn::Label("");
    add(lineLabels[i], 5, 5 + 15 * i);
  }
  
  hide();
}

void Console::addLine(const std::string line) {
  
  lines.push_back(line);
  
  for(int i = 0; i < MAX_LINES; i++) {
    int lineNum = lines.size() - MAX_LINES + i;
    if(lineNum >= 0 && lineNum < lines.size()) {
      lineLabels[i]->setCaption(lines[lineNum]);
      lineLabels[i]->adjustSize();
    }
  }
  
}

void Console::show() {
  setHeight(90);
  setVisible(true);
  showing = true;
}

void Console::hide() {
  setHeight(0);
  setVisible(false);
  showing = false;
}

void Console::toggle() {
  std::cout << "Toggle console " << showing << std::endl;
  if(showing) {
    hide();
  } else {
    show();
  }
}


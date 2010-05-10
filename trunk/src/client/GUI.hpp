

#ifndef _GUI_H_
#define _GUI_H_

#include <string>

class Client;

class GUI {
  private:
      
  public:

  virtual void init() = 0;
  virtual void update() = 0;
  virtual void render() = 0;
  virtual void handleEvent(SDL_Event &) = 0;
  
  virtual void console_log(std::string s) = 0;

  GUI() {
  
  }

  virtual ~GUI() {
  
  }

};

#endif

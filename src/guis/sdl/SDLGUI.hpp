#ifndef _SDLGUI_HPP_
#define _SDLGUI_HPP_

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#include "guichan.hpp"
#include "guichan/sdl.hpp"
#include "widgets/sdltruetypefont.hpp"
#include "widgets/Toolbar.hpp"
#include "widgets/MiniMap.hpp"
#include "widgets/Console.hpp"

#include "../../shared.h"
#include "../../GUI.hpp"
#include "../../Client.hpp"

#include "MapRender.hpp"
#include "BuildingRender.hpp"
#include "ToolRender.hpp"

#include <string>
#include <iostream>

class SDLGUI : public GUI, gcn::KeyListener, gcn::ActionListener, gcn::MouseListener {

  public:

  SDLGUI();
  ~SDLGUI();

  virtual void keyPressed(gcn::KeyEvent &keyEvent);
  virtual void keyReleased(gcn::KeyEvent &keyEvent);

  virtual void mouseDragged (gcn::MouseEvent &mouseEvent);
  virtual void mousePressed (gcn::MouseEvent &mouseEvent);
  virtual void mouseReleased (gcn::MouseEvent &mouseEvent);

  virtual void action(const gcn::ActionEvent &actionEvent);

  void console_log(std::string s);
  
  void render();
  void update();
  
  void run();
  
  private:
  
  gcn::SDLInput         *input;
  gcn::SDLGraphics      *graphics;
  gcn::SDLImageLoader   *imageLoader;
  
  gcn::Gui              *gui;
  gcn::Container        *top;
  gcn::contrib::SDLTrueTypeFont  *guiFont;
  Toolbar                        *toolbar;
  MiniMap                        *miniMap;
  Console                        *console;
  
  
  SDL_Surface *screen;
  Client      *client;
  
  SDL_Surface *menu_background;
  SDL_Surface *gui_background;
  
  SDL_Surface *mouse_block;
  SDL_Surface *mouse_pointer;
  
  MapRender       *mr;
  BuildingRender  *br;
  ToolRender      *tr;
  Camera          *camera;
  
  Point           mouse_down_tile;
  Point           mouse_up_tile;
  int             mx, my;
    
  bool usingTool;
  
  void init();
  
  bool                     showConsole;
  std::vector<std::string> consoleData;

};
#endif


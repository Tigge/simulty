#ifndef _TOOLRENDER_HPP_
#define _TOOLRENDER_HPP_

#include "shared.h"

#include "SDL.h"
#include "SDL_ttf.h"

#include "guichan.hpp"

#include "Camera.hpp"
#include "ImageLoader.hpp"
#include "MapRender.hpp"
#include "Client.hpp"

class ToolRender {

  private:
  
  Client      *client;
  MapRender   *mr;
  Camera      *camera;
  SDL_Surface *selector;
  gcn::Label  *costLabel;
  
  int          tool;
  Point        toolStartPos, toolEndPos;

  public:
  
  ToolRender(gcn::Gui *gui, Client *client, MapRender *mr, Camera *camera);
  virtual ~ToolRender();
  
  virtual void toolStart(int tool, Point start);
  virtual void toolUpdate(Point to);
  virtual void toolEnd();
  
  virtual void renderArea(SDL_Surface *on, MapRender *mr, Camera camera, Point from, Point to);
  virtual void renderTrail(SDL_Surface *on, MapRender *mr, Camera camera, Point from, Point to);
  virtual void render(SDL_Surface *on);
};

#endif

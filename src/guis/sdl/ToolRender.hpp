#ifndef _TOOLRENDER_HPP_
#define _TOOLRENDER_HPP_

#include "../../shared.h"

#include "SDL.h"
#include "SDL_ttf.h"

#include "guichan.hpp"

#include "Camera.hpp"
#include "ImageLoader.hpp"
#include "MapRender.hpp"
#include "../../Client.hpp"

class ToolRender {

  private:
  
  SDL_Surface *selector;
  gcn::Label  *costLabel;

  public:
  
  ToolRender(gcn::Gui *gui);
  virtual ~ToolRender();
  
  virtual void renderArea(SDL_Surface *on, MapRender *mr, Camera camera, Point from, Point to);
  virtual void renderTrail(SDL_Surface *on, MapRender *mr, Camera camera, Point from, Point to);
  virtual void render(SDL_Surface *on, MapRender *mr, Camera camera, Client *c, int tool, Point from, Point to);
};

#endif

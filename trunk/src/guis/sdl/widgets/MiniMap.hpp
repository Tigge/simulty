#ifndef _MINIMAP_HPP_
#define _MINIMAP_HPP_

#include "guichan.hpp"

#include "../../../Map.hpp"
#include "../../../Point.hpp"
#include "../Camera.hpp"

class MiniMap : public gcn::Widget, gcn::MouseListener {

  private:

  int mode;
  Map    *map;
  Camera *camera;

  public:
  
  MiniMap(Map *, Camera *);
  virtual ~MiniMap();  
  
  virtual void draw(gcn::Graphics *g);
  virtual void drawSelector(gcn::Graphics *g);

  virtual void mousePressed(gcn::MouseEvent &e);
  virtual void mouseDragged(gcn::MouseEvent &e);

};

#endif

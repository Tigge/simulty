#ifndef _MINIMAP_HPP_
#define _MINIMAP_HPP_

#include "../AllegroGUIShared.hpp"

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
  //void render(BITMAP *bitmap, Point where);
  
  //int getWidth();
  //int getHeight();
  
  virtual void mouseClicked(gcn::MouseEvent &e);

};

#endif

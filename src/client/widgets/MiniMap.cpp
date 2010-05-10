#include "MiniMap.hpp"



MiniMap::MiniMap(Map *map, Camera *camera) : gcn::Widget() {
  setWidth((map->getWidth() + map->getHeight() - 1) * 2);
  setHeight(map->getWidth() + map->getHeight() - 1);

  this->map    = map;
  this->camera = camera;

  addMouseListener(this);
}
MiniMap::~MiniMap() {

}

void MiniMap::draw(gcn::Graphics *g) {

  g->setColor(gcn::Color(150, 150, 150));
  g->fillRectangle(gcn::Rectangle(0, 0, getWidth(), getHeight()));

  Point base(map->getWidth() * 2, 0);

  for(unsigned int x = 0; x < map->getWidth(); x++) {
    for(unsigned int y = 0; y < map->getHeight(); y++) {
      Tile *t = map->getTile(x, y);
      
      if(t->isRoad()) {
        g->setColor(gcn::Color(100, 100, 100));
      } else if(t->getZone() == SIMULTY_ZONE_RES) {
        g->setColor(gcn::Color(78, 216, 24));
      } else if(t->getZone() == SIMULTY_ZONE_COM) {
        g->setColor(gcn::Color(113, 148, 227));
      } else if(t->getZone() == SIMULTY_ZONE_IND) {
        g->setColor(gcn::Color(222, 159, 49));
      } else {
        g->setColor(gcn::Color(112, 152, 40));
      }
      int bx = base.getX() + x + (y * 2);
      int by = base.getY() + y;

      g->drawPoint(bx, by);
      g->drawPoint(bx + 1, by);
            
      if(x != 0) {
        g->drawPoint(bx, by - 1);
      } if( x != map->getWidth() - 1) {
        g->drawPoint(bx + 1, by + 1);
      }

    }
    base.translate(-3, 1);
  }
  
  drawSelector(g);
}


void MiniMap::drawSelector(gcn::Graphics *g) {
  
  gcn::Rectangle r(camera->getX() / (TILE_W / 4), camera->getY() / (TILE_H / 2),
      640 / TILE_W * 4, 480 / TILE_H * 2);
  g->setColor(gcn::Color(255, 255, 255));
  g->drawRectangle(r);


}

void MiniMap::mousePressed(gcn::MouseEvent &e) {
  camera->setX(e.getX() * (TILE_W / 4));
  camera->setY(e.getY() * (TILE_H / 2));
}
void MiniMap::mouseDragged(gcn::MouseEvent &e) {
  camera->setX(e.getX() * (TILE_W / 4));
  camera->setY(e.getY() * (TILE_H / 2));
}



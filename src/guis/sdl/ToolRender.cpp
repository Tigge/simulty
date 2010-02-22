#include "ToolRender.hpp"

#include <iostream>

ToolRender::ToolRender(gcn::Gui *gui, Client *client, MapRender *mr, Camera *camera) {

  costLabel = new gcn::Label("asdfasdfasdf");
  ((gcn::Container *)gui->getTop())->add(costLabel);
  //costLabel->setVisible(false);
  costLabel->adjustSize();
  
  this->client = client;
  this->mr     = mr;
  this->camera = camera;
  
  try {
    selector = ImageLoader::getImage("img/selector.png");
  } catch(ImageLoaderException e) {
    std::cout << e.what() << std::endl;
    exit(1);
  }

}

ToolRender::~ToolRender() {
  SDL_FreeSurface(selector);
  free(costLabel);
}

void ToolRender::toolStart(int tool, Point start) {
  this->tool         = tool;
  this->toolStartPos = start;

  costLabel->setVisible(true);

  toolUpdate(start);
}

void ToolRender::toolUpdate(Point end) {
  this->toolEndPos = end;
  
  
  // Write text
  int cost = 0;
  
  Point from = toolStartPos;
  Point to   = toolEndPos;
  
  if(tool == SIMULTY_CLIENT_TOOL_ZONE_COM
      || tool == SIMULTY_CLIENT_TOOL_ZONE_RES
      || tool == SIMULTY_CLIENT_TOOL_ZONE_IND) {
    cost = client->map->buildZoneCost(client->getMyPlayer()->getSlot(), tool, from, to);
  } else if(tool == SIMULTY_CLIENT_TOOL_LAND) {
    cost = client->map->buyLandCost(client->getMyPlayer()->getSlot(), from, to);
  } else if(tool == SIMULTY_CLIENT_TOOL_ROAD) {
    cost = client->map->buildRoadCost(client->getMyPlayer()->getSlot(), from, to);
  } else if(tool == SIMULTY_CLIENT_TOOL_BULLDOZER) {
    cost = client->map->bulldozeCost(client->getMyPlayer()->getSlot(), from, to);
  } else if(tool == SIMULTY_CLIENT_TOOL_DEZONE) {
    cost = client->map->deZoneCost(client->getMyPlayer()->getSlot(), from, to);
  }
  
  // TODO: Does not work, need new Color label (I guess)
  if(cost > client->getMyPlayer()->getBudget()->getBalance()) {
    costLabel->setBackgroundColor(gcn::Color(255, 0, 0));
    costLabel->setBaseColor(gcn::Color(255, 0, 0));
    std::cout << "to much" << std::endl;
  } else {
    costLabel->setBackgroundColor(gcn::Color(0, 0, 0));
    costLabel->setBaseColor(gcn::Color(0, 0, 0));
  }
  
  char text[256];
  sprintf(text, "%i", cost);
  
  costLabel->setCaption(text);
  Point labelPos =  mr->toScreenCoord(toolEndPos, *camera);
  costLabel->setPosition(labelPos.getX(), labelPos.getY());
  
}

void ToolRender::toolEnd() {
  costLabel->setVisible(false);
}

void ToolRender::renderArea(SDL_Surface *on, MapRender *mr, Camera camera, Point from, Point to) {

  Point::fixOrder(from, to);

  SDL_Rect pos;
  for(int x = from.getX(); x <= to.getX(); x++) {
    for(int y = from.getY(); y <= to.getY(); y++) {
      
      Point sc = mr->toScreenCoord(Point(x, y), camera);
      pos.x = sc.getX();
      pos.y = sc.getY();
      SDL_BlitSurface(selector, NULL, on, &pos);
    }
  }
}

void ToolRender::renderTrail(SDL_Surface *on, MapRender *mr, Camera camera, Point from, Point to) {

  Point line;

  // XXX: fix, make more clear
  if((from.getX() - to.getX())*(from.getX() - to.getX() < 0 ? -1 : 1)
   > (from.getY() - to.getY())*(from.getY() - to.getY() < 0 ? -1 : 1)) {  // delta x > delta y
    line.setY(from.getY());
    line.setX(to.getX());
  }
  else {
    line.setX(from.getX());
    line.setY(to.getY());
  }

  Point::fixOrder(from, to);

  SDL_Rect pos;
  
  for(int x = from.getX(); x <= to.getX(); x++) {
    Point sc = mr->toScreenCoord(Point(x, line.getY()), camera);
    pos.x = sc.getX();
    pos.y = sc.getY();
    SDL_BlitSurface(selector, NULL, on, &pos);
  }

  for(int y = from.getY(); y <= to.getY(); y++) {
    Point sc = mr->toScreenCoord(Point(line.getX(), y), camera);
    pos.x = sc.getX();
    pos.y = sc.getY();
    SDL_BlitSurface(selector, NULL, on, &pos);  
  }
}


void ToolRender::render(SDL_Surface *on) {

  if(tool == -1) {
    return;
  }

  if(tool == SIMULTY_CLIENT_TOOL_ZONE_COM
      || tool == SIMULTY_CLIENT_TOOL_ZONE_RES
      || tool == SIMULTY_CLIENT_TOOL_ZONE_IND
      || tool == SIMULTY_CLIENT_TOOL_BULLDOZER
      || tool == SIMULTY_CLIENT_TOOL_LAND) {

    renderArea(on, mr, *camera, toolStartPos, toolEndPos);

  } else if(tool == SIMULTY_CLIENT_TOOL_ROAD) {
    renderTrail(on, mr, *camera, toolStartPos, toolEndPos);
  }
}




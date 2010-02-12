#include "ToolRender.hpp"

#include <iostream>

ToolRender::ToolRender(gcn::Gui *gui) {

  costLabel = new gcn::Label("asdfasdfasdf");
  ((gcn::Container *)gui->getTop())->add(costLabel);
  //costLabel->setVisible(false);
  costLabel->adjustSize();
  
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

void ToolRender::render(SDL_Surface *on, MapRender *mr, Camera camera, Client *client, int tool, Point from, Point to) {

  if(tool == SIMULTY_CLIENT_TOOL_ZONE_COM
      || tool == SIMULTY_CLIENT_TOOL_ZONE_RES
      || tool == SIMULTY_CLIENT_TOOL_ZONE_IND
      || tool == SIMULTY_CLIENT_TOOL_BULLDOZER
      || tool == SIMULTY_CLIENT_TOOL_LAND) {
      
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
  
  // Write text
  
  costLabel->setVisible(true);
  
  int cost = 0;
  
  if(tool == SIMULTY_CLIENT_TOOL_ZONE_COM
      || tool == SIMULTY_CLIENT_TOOL_ZONE_RES
      || tool == SIMULTY_CLIENT_TOOL_ZONE_IND) {
    cost = client->map->buildZoneCost(client->getMyPlayer()->getSlot(), tool, from, to);
    //std::cout << "z: " << cost << " " << c1 << c3 << std::endl;
  } else if(tool == SIMULTY_CLIENT_TOOL_LAND) {
    cost = client->map->buyLandCost(client->getMyPlayer()->getSlot(), from, to);
    //std::cout << "l: " << cost << " " << c1 << c3 << std::endl;
  } else if(tool == SIMULTY_CLIENT_TOOL_BULLDOZER) {
    cost = client->map->bulldozeCost(client->getMyPlayer()->getSlot(), from, to);
  } else if(tool == SIMULTY_CLIENT_TOOL_DEZONE) {
    cost = client->map->deZoneCost(client->getMyPlayer()->getSlot(), from, to);
  }
  
  if(cost > client->getMyPlayer()->getBudget()->getBalance())
    costLabel->setBaseColor(gcn::Color(255, 0, 0));
  else
    costLabel->setBaseColor(gcn::Color(0, 0, 0));
  
  char text[256];
  sprintf(text, "%i", cost);
  
  costLabel->setCaption(text);
  Point labelPos =  mr->toScreenCoord(to, camera);
  costLabel->setPosition(labelPos.getX(), labelPos.getY());
  
  //std::cout << "Moving label '" << text << "' to " << to << std::endl;
}




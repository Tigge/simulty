#include "SDLGUI.hpp"

#include "ImageLoader.hpp"
#include "ImageLoaderException.hpp"
#include "widgets/MainMenu.hpp"

int main(int argc, char *argv[]) {

  try {
    std::cerr << "Starting..." << std::endl;
    SDLGUI *gui = new SDLGUI();
    std::cerr << "New game client" << std::endl;

    gui->run();

    std::cerr << "Deleting game client..." << std::endl;
    delete gui;

    std::cerr << "Ending..." << std::endl;
  } catch (gcn::Exception &e) {
    std::cerr << e.getMessage() << std::endl;
    return 1;
  } catch (std::exception &e) {
    std::cerr << "Std exception: " << e.what() << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "Unknown exception" << std::endl;
    return 1;
  }
  return 0;
}


SDLGUI::SDLGUI() {
  init();
}

SDLGUI::~SDLGUI() {
  SDL_FreeSurface(screen);
}

void SDLGUI::init() {

  if(SDL_Init( SDL_INIT_EVERYTHING) == -1) {
    fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }
  
  screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
  
  if(TTF_Init()==-1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    exit(2);
  }
  
  SDL_EnableUNICODE(1);
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

  
  client = new Client(this);
  
  try {

    menu_background = ImageLoader::getImage("img/menubg.pcx");
    gui_background  = ImageLoader::getImage("img/guibg.pcx");

    mouse_pointer   = ImageLoader::getImage("img/cursor.pcx");
    mouse_block     = ImageLoader::getImage("img/mouse_block.pcx");

  } catch(ImageLoaderException e) {
    std::cerr << "Error: " << e.what();
    exit(1);
  }
  
  mr = new MapRender();
  mr->setMap(client->map);

  br = new BuildingRender();

  
  // Guichan:
  
  imageLoader = new gcn::SDLImageLoader();
  gcn::Image::setImageLoader(imageLoader);
  graphics = new gcn::SDLGraphics();
  graphics->setTarget(screen);
  input = new gcn::SDLInput();

  top = new gcn::Container();
  top->setDimension(gcn::Rectangle(0, 0, 640, 480));
  top->setOpaque(false);
  top->addMouseListener(this);

  gui = new gcn::Gui();
  gui->setGraphics(graphics);
  gui->setInput(input);
  gui->setTop(top);

  gui->addGlobalKeyListener(this);
  // Load the image font.
  guiFont = new gcn::contrib::SDLTrueTypeFont("DejaVuSans.ttf", 12);
  // The global font is static and must be set.
  gcn::Widget::setGlobalFont(guiFont);
  
  MainMenu *mainMenu = new MainMenu(this);
  top->add(mainMenu, 100, 100);
  
  toolbar       = new Toolbar();
  top->add(toolbar, screen->w - 10 - toolbar->getWidth(), 10);
  
  miniMap       = new MiniMap(client->map, &camera);
  top->add(miniMap, screen->w - miniMap->getWidth(),
      screen->h - miniMap->getHeight());
  
  console       = new Console();
  top->add(console, 0, 0);
  console->setWidth(screen->w);
  console->setHeight(100);  
  tr = new ToolRender(gui);
}

void SDLGUI::run() {
  while(true) {
    
    update();
    render();
      //rest(0);
  }
}


void SDLGUI::console_log(std::string s) {
  console->addLine(s);
}

void SDLGUI::update() {
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    input->pushInput(event);
  }
  
  gui->logic();
  client->update();
}

void SDLGUI::render() {

  SDL_FillRect(screen, NULL, 0);

  if(client->state_menu) {
    
    SDL_BlitSurface(menu_background, NULL, screen, NULL);
    client->state_menu = false; client->state_game = SIMULTY_CLIENT_STATE_GAME_START;
    
  } else if(client->state_game == SIMULTY_CLIENT_STATE_GAME_ON) {
    // TODO
    mr->render(screen, camera);
    // Render buildings:
    br->render(screen, mr, camera, &client->bman);
    
    if(usingTool) {
      tr->render(screen, mr, camera, client, toolbar->getTool(), mouse_down_tile, mouse_up_tile);
    }
  }
  
  gui->draw();
  
  //Update Screen
  SDL_Flip(screen);
  //SDL_Delay(1000);
}

void SDLGUI::mouseDragged (gcn::MouseEvent &e) {

  if(e.getSource() == top && e.getButton() == gcn::MouseEvent::RIGHT) {
    camera.translate(mx - e.getX(), my - e.getY());
    mx = e.getX();
    my = e.getY();
  } else {

    std::cout << "mouse dragged" << e.getX() << ", " << e.getY() << std::endl;

    if(e.getSource() == top) {
      Point p = mr->toTileCoord(Point(e.getX(), e.getY()), camera);
      if(p.getX() < 0)
        p.setX(0);
      if(p.getY() < 0)
        p.setY(0);
      if(p.getX() >= (int)client->map->getWidth())
        p.setX((int)client->map->getWidth() - 1);
      if(p.getY() >= (int)client->map->getHeight())
        p.setY((int)client->map->getHeight() - 1);

      mouse_up_tile = p;
    }
  }
}

void SDLGUI::mousePressed (gcn::MouseEvent &e) {

  if(e.getSource() == top && e.getButton() == gcn::MouseEvent::RIGHT) {
    mx = e.getX();
    my = e.getY();
  } else {

    if(e.getSource() == top) {
      Point p = mr->toTileCoord(Point(e.getX(), e.getY()), camera);
      
      std::cout << "Point: " << p << std::endl;
      
      if(!client->map->outOfBounds(p)) {
        mouse_down_tile = mouse_up_tile = p;
        usingTool       = true;
      }
      
      if(e.getButton() == gcn::MouseEvent::RIGHT) {
        client->debug(mr->toTileCoord(Point(e.getX(), e.getY()), camera));
      }
    }
  }
}

void SDLGUI::mouseReleased (gcn::MouseEvent &e) {
  if(e.getSource() == top && e.getButton() == gcn::MouseEvent::RIGHT) {
    
  } else {

    std::cout << "mouse dragged" << mouse_down_tile << ", " << mouse_up_tile << std::endl;

    if(usingTool) {

      int tool = toolbar->getTool();

      if(tool == SIMULTY_CLIENT_TOOL_LAND) {
          // buy land
          client->buyLand(mouse_down_tile, mouse_up_tile);
      } else if(tool == SIMULTY_CLIENT_TOOL_ROAD) {
          // draw road
          client->buyRoad(mouse_down_tile, mouse_up_tile);
      } else if(tool == SIMULTY_CLIENT_TOOL_ZONE_RES ||
              tool == SIMULTY_CLIENT_TOOL_ZONE_COM ||
              tool == SIMULTY_CLIENT_TOOL_ZONE_IND) {
            // zone
          client->buyZone(mouse_down_tile, mouse_up_tile, tool);
      } else if(tool == SIMULTY_CLIENT_TOOL_BUILD_POLICE) {
        client->buyBuilding(mouse_down_tile, Building::TYPE_POLICE);
      } else if(tool == SIMULTY_CLIENT_TOOL_BUILD_FIRE) {
        client->buyBuilding(mouse_down_tile, Building::TYPE_FIRE);
      } else if(tool == SIMULTY_CLIENT_TOOL_BUILD_HOSPITAL) {
        client->buyBuilding(mouse_down_tile, Building::TYPE_HOSPITAL);
      } else if(tool == SIMULTY_CLIENT_TOOL_BULLDOZER) {
        client->bulldoze(mouse_down_tile, mouse_up_tile);
      } else if(tool == SIMULTY_CLIENT_TOOL_DEZONE) {
        client->deZone(mouse_down_tile, mouse_up_tile);
      }
      usingTool = false;
    }
  }
}

void SDLGUI::keyPressed(gcn::KeyEvent &keyEvent) {
  if(keyEvent.getKey().getValue() == gcn::Key::UP) {
    camera.step(DIR_UP, 9, client->map->getWidth() * TILE_W - screen->w,
        client->map->getHeight() * TILE_H - screen->h);
  } else if(keyEvent.getKey().getValue() == gcn::Key::RIGHT) {
    camera.step(DIR_RIGHT, 9, client->map->getWidth() * TILE_W - screen->w,
        client->map->getHeight() * TILE_H - screen->h);
  } else if(keyEvent.getKey().getValue() == gcn::Key::DOWN) {
    camera.step(DIR_DOWN, 9, client->map->getWidth() * TILE_W - screen->w,
        client->map->getHeight() * TILE_H - screen->h);
  } else if(keyEvent.getKey().getValue() == gcn::Key::LEFT) {
    camera.step(DIR_LEFT, 9, client->map->getWidth() * TILE_W - screen->w,
        client->map->getHeight() * TILE_H - screen->h);
  }
}

void SDLGUI::keyReleased(gcn::KeyEvent &keyEvent) {
  std::cout << "key released - " << ((int)keyEvent.getKey().getValue()) << std::endl;
  
  if(keyEvent.getKey().getValue() == 167) {
    std::cout << "toggling console" << std::endl;
    console->toggle();
  }
  
}

void SDLGUI::action(const gcn::ActionEvent &actionEvent) {
  if(actionEvent.getId() == "mainmenu-exit") {
    exit(0);
  }
}



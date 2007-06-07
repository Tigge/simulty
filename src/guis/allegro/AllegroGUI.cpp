#include "AllegroGUI.hpp"

#include "../../Client.hpp"
#include "ImageLoader.hpp"

#include "../../LoaderSaver.hpp"

int main(int argc, char *argv[]) {

  try {
    std::cerr << "Starting..." << std::endl;
    AllegroGUI *gui = new AllegroGUI();
    std::cerr << "New game client" << std::endl;

    while(gui->running()) {

        //cerr << ".";
        while(gui->needUpdate())
        {
            gui->update();
        }

        gui->render();
        rest(0);
    }

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
} END_OF_MAIN()



// TODO - static functions instead? yes, do static
void AllegroGUI::handleSpeed(void *data) {
    ((AllegroGUI *)data)->speed_counter++;
} END_OF_FUNCTION(AllegroGUI::speedhandler)

void AllegroGUI::handleFPS(void *data) {
    ((AllegroGUI *)data)->fps    = ((AllegroGUI *)data)->frames;
    ((AllegroGUI *)data)->frames = 0;
} END_OF_FUNCTION(fpshandler)


AllegroGUI::AllegroGUI() {

  // Initializing allegro (and some sub elements)
  if(install_allegro(SYSTEM_AUTODETECT, &errno, atexit) != 0) {
    allegro_message("* Allegro could not be inited:\n  %s", allegro_error);
    exit(0);
  }

  set_color_depth(16);
  if(set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1024, 768, 0, 0) != 0) {
    allegro_message("* Graphics could not be inited:\n  %s", allegro_error);
    exit(0);
  }
  set_color_conversion(COLORCONV_TOTAL | COLORCONV_KEEP_TRANS);

  // No mouse, bitte!
  show_mouse(NULL);

  if(install_timer() != 0) {
    allegro_message("* Timers could not be inited:\n  %s", allegro_error);
    exit(0);
  } else if(install_keyboard() != 0) {
    allegro_message("* Keyboard could not be inited:\n  %s", allegro_error);
    exit(0);
  } else if(install_mouse() == -1) {
    allegro_message("* Mouse could not be inited:\n  %s", allegro_error);
    exit(0);
  }

  install_param_int_ex(AllegroGUI::handleSpeed, this, BPS_TO_TIMER(60));
  install_param_int_ex(AllegroGUI::handleFPS,   this, BPS_TO_TIMER(1));

  fps =  speed_counter =  frames = 0;

  // Locking variables and functions
  LOCK_VARIABLE(fps);
  LOCK_VARIABLE(speed_counter);
  LOCK_VARIABLE(frames);

  LOCK_FUNCTION(speedhandler);
  LOCK_FUNCTION(fpshandler);

  // Setting color depth
  std::cout << "Allegro inited..." << std::endl;

  // Create new client to work on
  client = new Client(this);

  // Create double buffer
  buffer = create_bitmap(SCREEN_W, SCREEN_H);

  if(!buffer) {
    allegro_message("Couldn't load / create some images");
    exit(1);
  }

  mr = new MapRender();
  mr->setMap(client->map);

  br = new BuildingRender();

  try {

    menu_background = ImageLoader::getImage("img/menubg.pcx");
    gui_background  = ImageLoader::getImage("img/guibg.pcx");

    mouse_pointer   = ImageLoader::getImage("img/cursor.pcx");
    mouse_block     = ImageLoader::getImage("img/mouse_block.pcx");



  } catch(ImageLoaderException e) {
    allegro_message("Error: %s", e.what());
    exit(1);
  }

  // Set up guichan:


  imageLoader = new gcn::AllegroImageLoader();
  gcn::Image::setImageLoader(imageLoader);

  graphics = new gcn::AllegroGraphics();
  graphics->setTarget(buffer);

  input = new gcn::AllegroInput();

  top = new gcn::Container();
  // Set the dimension of the top container to match the screen.
  top->setDimension(gcn::Rectangle(0, 0, SCREEN_W, SCREEN_H));
  // Make it transparent
  top->setOpaque(false);

  gui = new gcn::Gui();
  // Set gui to use the AllegroGraphics object.
  gui->setGraphics(graphics);
  // Set gui to use the AllegroInput object
  gui->setInput(input);
  // Set the top container
  gui->setTop(top);

  gui->addGlobalKeyListener(this);
  // Load the image font.
  //guiFont = new gcn::ImageFont("fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
  //guiFont = new gcn::DefaultFont();
  // The global font is static and must be set.
  //gcn::Widget::setGlobalFont(guiFont);

  indIcon   = imageLoader->load("img/menu_icon_ind.pcx", true);
  indButton = new gcn::ImageButton(indIcon);
  indButton->addActionListener(this);
  top->add(indButton, SCREEN_W - 74, 5);

  comIcon   = imageLoader->load("img/menu_icon_com.pcx", true);
  comButton = new gcn::ImageButton(comIcon);
  comButton->addActionListener(this);
  top->add(comButton, SCREEN_W - 37, 5);

  resIcon   = imageLoader->load("img/menu_icon_res.pcx", true);
  resButton = new gcn::ImageButton(resIcon);
  resButton->addActionListener(this);
  top->add(resButton, SCREEN_W - 74, 42);


  roadIcon  = imageLoader->load("img/menu_icon_road.pcx", true);
  roadButton = new gcn::ImageButton(roadIcon);
  roadButton->addActionListener(this);
  top->add(roadButton, SCREEN_W - 37, 42);

  landIcon  = imageLoader->load("img/menu_icon_land.pcx", true);
  landButton = new gcn::ImageButton(landIcon);
  landButton->addActionListener(this);
  top->add(landButton, SCREEN_W - 74, 79);


  policeIcon   = imageLoader->load("img/menu_icon_police.pcx", true);
  policeButton = new gcn::ImageButton(policeIcon);
  policeButton->addActionListener(this);
  top->add(policeButton, SCREEN_W - 37, 79);

  fireIcon  = imageLoader->load("img/menu_icon_fire.pcx", true);
  fireButton = new gcn::ImageButton(fireIcon);
  fireButton->addActionListener(this);
  top->add(fireButton, SCREEN_W - 74, 116);

  hospitalIcon  = imageLoader->load("img/menu_icon_hospital.pcx", true);
  hospitalButton = new gcn::ImageButton(hospitalIcon);
  hospitalButton->addActionListener(this);
  top->add(hospitalButton, SCREEN_W - 37, 116);

  bulldozerIcon  = imageLoader->load("img/menu_icon_bulldozer.pcx", true);
  bulldozerButton = new gcn::ImageButton(bulldozerIcon);
  bulldozerButton->addActionListener(this);
  top->add(bulldozerButton, SCREEN_W - 74, 153);

  console_show = false;
  tool = 0;

  gcn::Window *w = new gcn::Window("Economy");
  w->setDimension(gcn::Rectangle(0, 0, 300, 300));
  w->setBaseColor(gcn::Color(255, 150, 200, 190));

  gcn::Slider *s = new gcn::Slider(0.0, 100.0);
  s->setSize(100, 10);

  w->add(s);
  top->add(w, 100, 100);


}

AllegroGUI::~AllegroGUI(){

  delete client;

  delete mr;
  delete br;

  destroy_bitmap(menu_background);
  destroy_bitmap(gui_background);

  destroy_bitmap(mouse_pointer);
  destroy_bitmap(mouse_block);

  destroy_bitmap(buffer);

  delete roadIcon;
  delete roadButton;

  delete landIcon;
  delete landButton;

  delete policeIcon;
  delete policeButton;

  delete fireIcon;
  delete fireButton;

  delete hospitalIcon;
  delete hospitalButton;

  delete bulldozerIcon;
  delete bulldozerButton;

  // Guichan stuff
  delete input;
  delete graphics;
  delete imageLoader;

  delete resButton;
  delete comButton;
  delete indButton;

  delete resIcon;
  delete comIcon;
  delete indIcon;

  //delete guiFont;
  delete top;
  delete gui;

}

void AllegroGUI::keyPressed(gcn::KeyEvent &keyEvent) {
  std::cout << "KP: " << keyEvent.getKey().getValue() << std::endl;
}
void AllegroGUI::keyReleased(gcn::KeyEvent &keyEvent) {

  if(keyEvent.getKey().getValue() == gcn::Key::ESCAPE) {
    client->state_running = false;
  } else if(keyEvent.getKey().getValue() == gcn::Key::F1) {
    console_show = !console_show;
  }

  std::cout << "KR: " << keyEvent.getKey().getValue() << std::endl;
}

void AllegroGUI::action(const gcn::ActionEvent &actionEvent) {

  if(actionEvent.getSource() == comButton) {
    tool = SIMULTY_CLIENT_TOOL_ZONE_COM;
  } else if(actionEvent.getSource() == resButton) {
    tool = SIMULTY_CLIENT_TOOL_ZONE_RES;
  } else if(actionEvent.getSource() == indButton) {
    tool = SIMULTY_CLIENT_TOOL_ZONE_IND;
  } else if(actionEvent.getSource() == roadButton) {
    tool = SIMULTY_CLIENT_TOOL_ROAD;
  } else if(actionEvent.getSource() == landButton) {
    tool = SIMULTY_CLIENT_TOOL_LAND;
  } else if(actionEvent.getSource() == policeButton) {
    tool = SIMULTY_CLIENT_TOOL_BUILD_POLICE;
  } else if(actionEvent.getSource() == fireButton) {
    tool = SIMULTY_CLIENT_TOOL_BUILD_FIRE;
  } else if(actionEvent.getSource() == hospitalButton) {
    tool = SIMULTY_CLIENT_TOOL_BUILD_HOSPITAL;
  } else if(actionEvent.getSource() == bulldozerButton) {
    tool = SIMULTY_CLIENT_TOOL_BULLDOZER;
  }
}


void AllegroGUI::render() {

  // Clear the double buffer:
  clear_bitmap(buffer);

  if(client->state_menu) {

    blit(menu_background, buffer, 0, 0, SCREEN_W - menu_background->w,
        SCREEN_H - menu_background->h, menu_background->w, menu_background->h);

    textprintf_ex(buffer, font, SCREEN_W - 200, SCREEN_H - 300, makecol(0, 0, 0), -1, "New local game");
    textprintf_ex(buffer, font, SCREEN_W - 200, SCREEN_H - 280, makecol(0, 0, 0), -1, "New network game");
    textprintf_ex(buffer, font, SCREEN_W - 200, SCREEN_H - 260, makecol(0, 0, 0), -1, "Join network game");

    textprintf_ex(buffer, font, SCREEN_W - 200, SCREEN_H - 200, makecol(0, 0, 0), -1, "Quit");


  } else if(client->state_game == SIMULTY_CLIENT_STATE_GAME_ON) {

    // Render map:
    mr->render(buffer, camera);

    // Render buildings:
    br->render(buffer, mr, camera, &client->bman);

    if(mouse.getLeftButtonState() == STATE_PRESS) {


    } if(mouse.getLeftButtonState() == STATE_HOLD) {

      Point c1 = mouse_down_tile;
      Point c3 = mouse_up_tile;

      if(true) {//tool != SIMULTY_CLIENT_TOOL_ROAD) {

        Point::fixOrder(c1, c3);
        //c3.translate(1, 1);

        Point c2 = Point(c3.getX(), c1.getY());
        Point c4 = Point(c1.getX(), c3.getY());

        c1 = mr->toScreenCoord(c1, camera); c2 = mr->toScreenCoord(c2, camera);
        c3 = mr->toScreenCoord(c3, camera); c4 = mr->toScreenCoord(c4, camera);

        int points[8] = { c1.getX() + TILE_W / 2, c1.getY(), // Upper left corner
                          c2.getX(), c2.getY() + TILE_H / 2,
                          c3.getX() + TILE_W / 2, c3.getY() + TILE_H, // Lower right corner
                          c4.getX() + TILE_W, c4.getY() + TILE_H / 2};

        polygon(buffer, 4, points, makecol(255, 255, 255));
      }
      else {
        Point c2 = Point(c3.getX(), c1.getY());

        c1 = mr->toScreenCoord(c1, camera); c2 = mr->toScreenCoord(c2, camera);
        c3 = mr->toScreenCoord(c3, camera);

        int horizontalRoad[8] = { c2.getX(), c1.getY()+ TILE_H/2,
                                  c2.getX(), c1.getY(),
                                  c1.getX() + TILE_W, c1.getY() + TILE_H/2,
                                  c1.getX() + TILE_W/2, c1.getY()
                                  };

        //polygon(buffer, 4, verticalRoad, makecol(255, 255, 255));
        polygon(buffer, 4, horizontalRoad, makecol(255, 255, 255));
      }
    }
    //blit(mouse_hint, buffer, 0, 0, (mouse_x / TILE_W) * TILE_W, (mouse_y / TILE_H) * TILE_H, mouse_hint->w, mouse_hint->h);

    Point realtile = mr->toTileCoord(mouse.getPosition(), camera);
    Point realscrn = mr->toScreenCoord(realtile, camera);

    // Redner mouse block
    masked_blit(mouse_block, buffer, 0, 0, realscrn.getX(), realscrn.getY(), mouse_block->w, mouse_block->h);

    // Render GUI:
    masked_blit(gui_background, buffer, 0, 0, SCREEN_W - gui_background->w,
        SCREEN_H - gui_background->h, gui_background->w, gui_background->h);

    //textprintf_ex(buffer, font, 20, SCREEN_H - 40, makecol(0, 0, 0), -1, "Money: %i", client->money);
    textprintf_ex(buffer, font, 20, SCREEN_H - 30, makecol(0, 0, 0), -1, "Time: %i %s %i",
        client->cal.getYear(), client->cal.getMonthAsString().c_str(), client->cal.getDay());
    textprintf_ex(buffer, font, 20, SCREEN_H - 20, makecol(0, 0, 0), -1, "Tool: %i", tool);

    textprintf_ex(buffer, font, 200, SCREEN_H - 20, makecol(0, 0, 0), -1, "FPS: %i", fps);

    //textprintf_ex(buffer, font, 200, SCREEN_H - 50, makecol(0, 0, 0), -1, "Camera: %i, %i", camera.getX(), camera.getY());
    textprintf_ex(buffer, font, 200, SCREEN_H - 30, makecol(0, 0, 0), -1, "Mouse: %i, %i", realtile.getX(), realtile.getY());


    if(realtile.getX() > 5 && realtile.getY() > 5 && realtile.getX() < 25 && realtile.getY() < 25)
      textprintf_ex(buffer, font, 300, SCREEN_H - 30, makecol(0, 0, 0), -1, "Thrive: %i", client->bman.getThriveValue(client->map, client->player_me->getSlot(), realtile));


    textprintf_ex(buffer, font, 600, SCREEN_H - 30, makecol(0, 0, 0), -1, "MD: %i, %i MU: %i, %i", mouse_down_tile.getX(), mouse_down_tile.getY(), mouse_up_tile.getX(), mouse_up_tile.getY());
    textprintf_ex(buffer, font, 600, SCREEN_H - 60, makecol(0, 0, 0), -1, "SB: %i", client->bman.getSpecialBuildingCount());

    // Draw console:
    if(console_show)
    {
        rectfill(buffer, 0, 0, SCREEN_W, 100, makecol(50, 50, 50));

        for(int i = 1; i <= 5; i++)
        {
            if(console_data.size() - i >= 0 && console_data.size() - i < console_data.size())
                textprintf_ex(buffer, font, 10, 90 - 15*i, makecol(255, 255, 255), -1, "> %s", console_data[console_data.size() - i].c_str());
        }
    }

  }

  gui->draw();

  // Draw mouse pointer:
  masked_blit(mouse_pointer, buffer, 0, 0, mouse.getPosition().getX(), mouse.getPosition().getY(), 32, 32);

  // Render buffer to screen:
  blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

  // Increase number of frames:
  frames++;

}


void AllegroGUI::update()
{

  gui->logic();
  client->update();

    mouse.update();

    if(client->state_menu) {
      if(mouse.getLeftButtonState() == STATE_PRESS) {
        client->state_menu = false; client->state_game = SIMULTY_CLIENT_STATE_GAME_START;
      }
    } else if(client->state_game == SIMULTY_CLIENT_STATE_GAME_ON) {

        // Mouse input:
        if(mouse.getLeftButtonState() == STATE_PRESS) {

            mouse_down_tile = mr->toTileCoord(mouse.getPressPosition(), camera);



            //std::cout << "mouse press event" << std::endl;

        } else if(mouse.getLeftButtonState() == STATE_HOLD) {

            mouse_up_tile = mr->toTileCoord(mouse.getPosition(), camera);

            //std::cout << "mouse hold event" << std::endl;

        } else if(mouse.getLeftButtonState() == STATE_RELEASE) {

            //std::cout << "mouse release event" << std::endl;

            //Point::fixOrder(mouse_down_tile, mouse_up_tile);

            if(mouse.getPosition().getX() < SCREEN_W - 80) {

              if(tool == SIMULTY_CLIENT_TOOL_LAND) {
                  client->buyLand(mouse_down_tile, mouse_up_tile);
                  // buy land
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
              }
            }
        }

        if(key[KEY_UP]    || mouse_y < 15           )camera.step(DIR_UP,    3, client->map->getWidth() * TILE_W / 2, client->map->getHeight() * TILE_H / 2);
        if(key[KEY_RIGHT] || mouse_x > SCREEN_W - 15)camera.step(DIR_RIGHT, 3, client->map->getWidth() * TILE_W / 2, client->map->getHeight() * TILE_H / 2);
        if(key[KEY_DOWN]  || mouse_y > SCREEN_H - 15)camera.step(DIR_DOWN,  3, client->map->getWidth() * TILE_W / 2, client->map->getHeight() * TILE_H / 2);
        if(key[KEY_LEFT]  || mouse_x < 15           )camera.step(DIR_LEFT,  3, client->map->getWidth() * TILE_W / 2, client->map->getHeight() * TILE_H / 2);

        if(keypressed()) {
            if(key[KEY_F1])console_show = !console_show;

            if(key[KEY_PLUS_PAD])tool++;
            if(key[KEY_MINUS_PAD])tool--;


            if(key[KEY_S]) {
              std::cout << "Saving..." << std::endl;
              test = LoaderSaver::saveGame(client->map, NULL, NULL);
              //std::cout << test << std::endl;
            }
            if(key[KEY_L]) {
              std::cout << "Loading..." << std::endl;
              LoaderSaver::loadGame(test, client->map, NULL, NULL);
            }
            clear_keybuf();
        }

    }

  speed_counter--;

}


void AllegroGUI::console_log(std::string s) {
    console_data.push_back(s);
}

bool AllegroGUI::needUpdate() {
    return speed_counter > 0;
}

bool AllegroGUI::running() {
  return client->state_running;
}


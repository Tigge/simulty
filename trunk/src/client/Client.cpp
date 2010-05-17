
#include "shared.h"

#include "Client.hpp"
#include "SDLGUI.hpp"
#include "LoaderSaver.hpp"

#define err std::cerr


int main(int argc, char *argv[]) {

  try {
    std::cerr << "Starting..." << std::endl;
    Client client;
    std::cerr << "New game client" << std::endl;
    
    while(true) {
        //std::cerr << "Update" << std::endl;
        client.update();
        //std::cerr << "Render" << std::endl;
        client.render();
    }

    std::cerr << "Deleting game client..." << std::endl;

    std::cerr << "Ending..." << std::endl;
  } catch (gcn::Exception &e) {
    std::cerr << e.getMessage() << std::endl;
    return 1;
  } catch (std::exception &e) {
    std::cerr << "Std exception: " << e.what() << std::endl;
    return 1;
  } catch (std::string &e) {
    std::cerr << "String exception: " << e << std::endl;
  } catch (char *e) {
    std::cerr << "CString exception: " << e << std::endl;
  } catch (...) {
    std::cerr << "Unknown exception" << std::endl;
    return 1;
  }
  return 0;
}


Client::Client() {

    // Client is using this GUI:
    this->gui = new SDLGUI(this);
    
    init();
}

Client::~Client (){

  delete map;

}

void Client::init() {

    state_running = true;
    state_menu  = true;
    state_game = false;

    map = new Map(50, 50);

    myPlayer = NULL;
    
    gui->init();
    
    // Add local socket and connect it to server (TODO: move later)
    try {
        socket = new NL::Socket("localhost", 5557);
        
        sender = new PacketSender(socket);
        sender->start();
        receiver = new PacketReceiver(socket, USEREVENT_GOTPACKET, NULL);
        receiver->start();
    } catch(NL::Exception e) {
        std::cerr << "Can't connect to local server" << std::endl;
        exit(0);    
    }	

}

void Client::render() {
    gui->render();
}

void Client::update() {

    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_USEREVENT) {
            if(event.user.code == USEREVENT_GOTPACKET) {
                PacketReceiver *receiver = (PacketReceiver *)event.user.data2;
                //std::cout << "Got packet in main - ";
                packet_handle(receiver->popPacket());

            } else {
                //TODO
            }
        } else if(event.type == SDL_QUIT) {
            //TODO
        } else {
            gui->handleEvent(event); //TODO: input->pushInput(event);
        }
        //std::cout << ",";
    }
    //std::cout << ".";
    gui->update();

}

PlayerClientLocal *Client::getMyPlayer() {
    return myPlayer;
}

void Client::bulldoze(Point from, Point to) {

    gui->console_log("Bulldoze requested");
    std::cout << "SENDING BULLDOZE REQUEST" << std::endl;

    NL::Packet packet(NLPACKET_TYPE_SIMULTY_REQUEST_BULLDOZE);

    packet << (NLINT32)from.getX() << (NLINT32)from.getY()
           << (NLINT32)to.getX()   << (NLINT32)to.getY();

    sender->pushPacket(packet);

}

void Client::deZone(Point from, Point to) {

  gui->console_log("DeZone requested");

  NL::Packet packet(NLPACKET_TYPE_SIMULTY_REQUEST_DEZONE);

  packet << (NLINT32)from.getX() << (NLINT32)from.getY()
         << (NLINT32)to.getX()   << (NLINT32)to.getY();

  sender->pushPacket(packet);

}


void Client::buyLand(Point from, Point to) {

  gui->console_log("Land requested");

  NL::Packet packet(NLPACKET_TYPE_SIMULTY_REQUEST_LAND);
  packet << (NLINT32)from.getX() << (NLINT32)from.getY()
         << (NLINT32)to.getX()   << (NLINT32)to.getY();

  sender->pushPacket(packet);
}

void Client::buyRoad(Point from, Point to) {

  gui->console_log("Road requested");

  // Only straight lines (for now):
  //if(from.getX() == to.getX() || from.getY() == to.getY()) {
  NL::Packet packet(NLPACKET_TYPE_SIMULTY_REQUEST_ROAD);
  packet << (NLINT32)from.getX() << (NLINT32)from.getY()
         << (NLINT32)to.getX()   << (NLINT32)to.getY();

  sender->pushPacket(packet);
  //}
}

void Client::buyZone(Point from, Point to, int type) {

  gui->console_log("Zone requested");

  NL::Packet packet(NLPACKET_TYPE_SIMULTY_REQUEST_ZONE);

  packet << (NLINT16)type
         << (NLINT32)from.getX() << (NLINT32)from.getY()
         << (NLINT32)to.getX()   << (NLINT32)to.getY();

  sender->pushPacket(packet);

}


void Client::buyBuilding(Point where, int type) {

  gui->console_log("Building requested");

  NL::Packet packet(NLPACKET_TYPE_SIMULTY_REQUEST_SPECIAL_BUILDING);

  packet << (NLINT16)type
         << (NLINT32)where.getX() << (NLINT32)where.getY();

  sender->pushPacket(packet);
}

void Client::debug(Point p) {

  gui->console_log("Debugging");
  
  NL::Packet packet(NLPACKET_TYPE_SIMULTY_REQUEST_DEBUG);  
  packet << (NLINT32)p.getX() << (NLINT32)p.getY();
  sender->pushPacket(packet);

}

void Client::packet_handle(NL::Packet p) {

  switch(p.getType()) {

    case NLPACKET_TYPE_SIMULTY_WELCOME: {

      std::cerr << "** Got welcome message" << std::endl;

      std::string welcome; p >> welcome;

      NL::Packet ver(NLPACKET_TYPE_SIMULTY_VERSION_CLIENT);
      ver << (NLINT16)0 << (NLINT16)0 << (NLINT16)1;
      sender->pushPacket(ver);

      //alert("Welcome message from server", welcome.c_str(), NULL, "Ok", NULL, 0, 0);

      break;
    }
    case NLPACKET_TYPE_SIMULTY_GAMEDATA: {

      //std::string data;
      //data = p.getData();
      //p.print();
      //std::cout << "The data is " << data.length() << " chars long and packet is " << p.getSize() << std::endl;
      LoaderSaver::loadGame(p.getData(), map, NULL, NULL);
      break;
    }
    case NLPACKET_TYPE_SIMULTY_ID: {

      std::cerr << "** Got id message (update)" << std::endl;

      // Fetch ID and slot from packet:
      NLINT32 id_new; NLINT16 slot_new; p >> id_new >> slot_new;

      // Create new local player and assign id and slot:
      PlayerClientLocal *pl = new PlayerClientLocal(id_new, slot_new);
      //pl->get_id() = id_new; pl->slot = slot_new;

      // Add to player manager:
      pman.add(pl);

      // Set the player to our local player:
      myPlayer = pl;

      state_game = SIMULTY_CLIENT_STATE_GAME_ON;

      std::cerr << "*** ID is " << id_new << " and slot is " << slot_new << std::endl;

      break;
    }
    case NLPACKET_TYPE_SIMULTY_ROAD: {

      int   sl = p.nextInt16();
      Point fr = Point::fromPacket(p);
      Point to = Point::fromPacket(p);

      // TODO
      map->buildRoad(sl, fr, to);
      gui->console_log("Road built");
      break;
    }

    case NLPACKET_TYPE_SIMULTY_TIME_INCR: {
      std::cout << "time" << std::endl;
      date.advance();
      bman.distributePower();
      break;
    }
    case NLPACKET_TYPE_SIMULTY_BUDGET_UPDATE: {

      NLINT16 playerAffected;
      NLINT16 budgetMonth;
      NLINT16 budgetItem;
      NLINT32 balanceUpdate;

      p >> playerAffected >> budgetMonth >> budgetItem >> balanceUpdate;

      if(playerAffected == getMyPlayer()->getSlot()) {
        getMyPlayer()->getBudget()->updateBudgetItem(budgetItem, 
            budgetMonth, balanceUpdate);
        gui->console_log("My budget changed");
      }
      
      std::cout << "Budget changed. Item: " << budgetItem << " Month: " 
          << budgetMonth << ", Amount: " << balanceUpdate << std::endl;

      gui->console_log("Budget changed");

      break;
    }
    case NLPACKET_TYPE_SIMULTY_LAND: {

      std::cout << "land" << std::endl;
      int   sl = p.nextInt16();
      Point fr = Point::fromPacket(p);
      Point to = Point::fromPacket(p);
      map->buyLand(sl, fr, to);
      gui->console_log("Land bought");

      break;
    }
    case NLPACKET_TYPE_SIMULTY_ZONE: {

      int   sl = p.nextInt16();
      int   zt = p.nextInt16();
      Point fr = Point::fromPacket(p);
      Point to = Point::fromPacket(p);

      map->buildZone(sl, zt, fr, to);

      gui->console_log("Zone bought");

      break;
    }
    case NLPACKET_TYPE_SIMULTY_PLAYER_JOINED: {

      std::string nick; NLINT32 id; NLINT16 slot;
      p >> id >> slot >> nick;


      gui->console_log("Player joined");

      break;

    }
    case NLPACKET_TYPE_SIMULTY_PLAYER_LEFT: {

      NLINT32 id;
      p >> id;

      gui->console_log("Player left");

      break;
    }
    case NLPACKET_TYPE_SIMULTY_BUILD_SPECIAL_BUILDING: {

      NLINT16 buildingType, slot; NLINT32 x, y;
      p >> slot >> buildingType >> x >> y;

      std::cerr << "BC: " << buildingType << " " << Point(x, y) << std::endl;

      Building *b = BuildingFactory::getBuilding(buildingType,
          Point(x, y), (unsigned char)slot, date);

      bman.addSpecialBuilding(map, b);

      gui->console_log("Building built");

      break;
    }
    case NLPACKET_TYPE_SIMULTY_REMOVE_SPECIAL_BUILDING: {

      bman.removeSpecialBuilding(map, bman.getSpecialBuildingID(Point::fromPacket(p)));

      gui->console_log("Building razed");
      break;
    }
    case NLPACKET_TYPE_SIMULTY_BUILD_ZONE_BUILDING: {

      int playerSlot   = p.nextInt16();
      int buildingType = p.nextInt16();
      Point where      = Point::fromPacket(p);
      int w            = p.nextInt16();
      int h            = p.nextInt16();
      int level        = p.nextInt16();
      int style        = p.nextInt16();

      bman.addZoneBuilding(map, playerSlot, buildingType, where, w, h,
          date, level, style);

      break;
    }
    case NLPACKET_TYPE_SIMULTY_REMOVE_ZONE_BUILDING: {

      bman.removeZoneBuilding(map, bman.getZoneBuildingID(Point::fromPacket(p)));
      break;
    }
    case NLPACKET_TYPE_SIMULTY_BULLDOZE: {

      std::cout << "GOT BULLDOZE PACKET" << std::endl;

      int   sl = p.nextInt16();
      Point fr = Point::fromPacket(p);
      Point to = Point::fromPacket(p);

      map->bulldoze(sl, fr, to);
      bman.bulldoze(sl, map, fr, to);

      gui->console_log("Bulldozing");
      break;
    }
    
    case NLPACKET_TYPE_SIMULTY_DEZONE: {

      int   sl = p.nextInt16();
      Point fr = Point::fromPacket(p);
      Point to = Point::fromPacket(p);

      map->deZone(sl, fr, to);

      gui->console_log("DeZoneing");
      break;
    }
    
    default: {

      std::stringstream tmp;
      tmp << "** Got uknown message with id " << p.getType();
      gui->console_log(tmp.str().c_str());

      p.print();
      break;
    }
  }
}

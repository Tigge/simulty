
#include "shared.h"

#include "Client.hpp"
#include "LoaderSaver.hpp"

#define err std::cerr



Client::Client(GUI *gui) {

    // Client is using this GUI:
    this->gui = gui;

    // Add local socket and connect it to server (TODO: move later)
    net_client = net.add();
    if(!net_client->connect_to("localhost", 5557)) {
      std::cerr << "Can't connect to local server" << std::endl;
      exit(0);
    }

    state_running = true;
    state_menu  = true;
    state_game = false;

    map = new Map(50, 50);

    time = 0;

    // Set max players and set up all slots:

    player_me = NULL;

}

Client::~Client (){

  delete map;

}

void Client::update() {

  if(state_menu) {

  } else if(state_game) {

    net.update(0);

    if(net_client->packet_exists()) {
      //err << "* Client have recieved a package... " << endl;
      packet_handle(net_client->packet_get());

    }
  }
}


void Client::bulldoze(Point from, Point to) {

  NLPacket packet(NLPACKET_TYPE_SIMULTY_REQUEST_BULLDOZE);

  packet << (NLINT32)from.getX() << (NLINT32)from.getY()
         << (NLINT32)to.getX()   << (NLINT32)to.getY();

  net_client->packet_put(packet);

}

void Client::buyLand(Point from, Point to) {

  gui->console_log("Bought land (request)");

  NLPacket packet(NLPACKET_TYPE_SIMULTY_REQUEST_LAND);
  packet << (NLINT32)from.getX() << (NLINT32)from.getY()
         << (NLINT32)to.getX()   << (NLINT32)to.getY();

  net_client->packet_put(packet);
}

void Client::buyRoad(Point from, Point to) {

  // Only straight lines (for now):
  if(from.getX() == to.getX() || from.getY() == to.getY()) {
    NLPacket packet(NLPACKET_TYPE_SIMULTY_REQUEST_ROAD);
    packet << (NLINT32)from.getX() << (NLINT32)from.getY()
           << (NLINT32)to.getX()   << (NLINT32)to.getY();

    net_client->packet_put(packet);
  }
}

void Client::buyZone(Point from, Point to, int type) {

  gui->console_log("Bought zone (request)");

  NLPacket packet(NLPACKET_TYPE_SIMULTY_REQUEST_ZONE);

  packet << (NLINT16)type
         << (NLINT32)from.getX() << (NLINT32)from.getY()
         << (NLINT32)to.getX()   << (NLINT32)to.getY();

  net_client->packet_put(packet);

}


void Client::buyBuilding(Point where, int type) {

  NLPacket packet(NLPACKET_TYPE_SIMULTY_REQUEST_SPECIAL_BUILDING);

  packet << (NLINT16)type
         << (NLINT32)where.getX() << (NLINT32)where.getY();

  net_client->packet_put(packet);

  gui->console_log("Built building (request)");
}

void Client::packet_handle(NLPacket p) {

  switch(p.getType()) {

    case NLPACKET_TYPE_SIMULTY_WELCOME: {

      std::cerr << "** Got welcome message" << std::endl;

      std::string welcome; p >> welcome;

      NLPacket ver(NLPACKET_TYPE_SIMULTY_VERSION_CLIENT);
      ver << (NLINT16)0 << (NLINT16)0 << (NLINT16)1;
      net_client->packet_put(ver);

      //alert("Welcome message from server", welcome.c_str(), NULL, "Ok", NULL, 0, 0);

      break;
    }
    case NLPACKET_TYPE_SIMULTY_GAMEDATA: {

      std::string data;
      data = p.getData();
      p.print();
      std::cout << "The data is " << data.length() << " chars long and packet is " << p.getSize() << std::endl;
      LoaderSaver::loadGame(p.getData(), map, NULL, NULL);
      break;
    }
    case NLPACKET_TYPE_SIMULTY_ID: {

      std::cerr << "** Got id message (update)" << std::endl;

      // Fetch ID and slot from packet:
      NLINT32 id_new; NLINT16 slot_new; p >> id_new >> slot_new;

      // Create new local player and assign id and slot:
      player_client_local *pl = new player_client_local(id_new, slot_new);
      //pl->get_id() = id_new; pl->slot = slot_new;

      // Add to player manager:
      pman.add(pl);

      // Set the player to our local player:
      player_me = pl;

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
      gui->console_log("Built road");
      break;
    }

    case NLPACKET_TYPE_SIMULTY_TIME_INCR: {

      cal.advance();
      time++;
      break;
    }
    case NLPACKET_TYPE_SIMULTY_MONEY_CHANGE: {

      NLINT16 player_affected; NLINT32 money_new;
      p >> player_affected >> money_new;

      if(player_affected == player_me->getSlot()) {
        player_me->setMoney(money_new);
      }

      gui->console_log("Money changed");

      break;
    }
    case NLPACKET_TYPE_SIMULTY_LAND: {

      int   sl = p.nextInt16();
      Point fr = Point::fromPacket(p);
      Point to = Point::fromPacket(p);
      map->buyLand(sl, fr, to);
      gui->console_log("Bought land");

      break;
    }
    case NLPACKET_TYPE_SIMULTY_ZONE: {

      int   sl = p.nextInt16();
      int   zt = p.nextInt16();
      Point fr = Point::fromPacket(p);
      Point to = Point::fromPacket(p);

      map->buildZone(sl, zt, fr, to);

      gui->console_log("Bought zone");

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
    case NLPACKET_TYPE_SIMULTY_SPECIAL_BUILDING: {

      NLINT16 buildingType, slot; NLINT32 x, y;
      p >> slot >> buildingType >> x >> y;

      std::cerr << "BC: " << buildingType << " " << Point(x, y) << std::endl;

      Building *b = BuildingFactory::getBuilding(buildingType, Point(x, y), slot);
      bman.addSpecialBuilding(b);

      gui->console_log("Built building");

      break;
    }
    case NLPACKET_TYPE_SIMULTY_BULLDOZE: {

      int   sl = p.nextInt16();
      Point fr = Point::fromPacket(p);
      Point to = Point::fromPacket(p);
      
      map->bulldoze(sl, fr, to);
    
      std::cerr << "** I want to bulldoze, but don't know how to!" << std::endl;
      break;
    }
    default: {

      std::cerr << "** Got uknown message with id " << p.getType() << std::endl;
      p.print();
      break;
    }
  }
}


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
      //allegro_message(" * Could not connect to server");
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


void Client::buy_land(Point from, Point to) {

    gui->console_log("Bought land (request)");

    std::cout << "buy from "
              << from.getX() << ", " << from.getY() << " to "
              << to.getX() << ", "   << to.getY() << std::endl;

    NLPacket landpak(NPACKET_TYPE_SIMULTY_LAND_BUY);
    landpak << (NLINT32)from.getX() << (NLINT32)from.getY()
            << (NLINT32)to.getX()   << (NLINT32)to.getY();

    net_client->packet_put(landpak);
}

void Client::buy_road(Point from, Point to) {

    // Only straight lines (for now):
    if(from.getX() == to.getX() || from.getY() == to.getY()) {

        NLPacket roadpak(NPACKET_TYPE_SIMULTY_ROAD_BUILD);
        roadpak << (NLINT32)from.getX() << (NLINT32)from.getY()
                << (NLINT32)to.getX()   << (NLINT32)to.getY();
        net_client->packet_put(roadpak);

    }

}

void Client::buy_zone(Point from, Point to, int type) {

    gui->console_log("Bought zone (request)");

    NLPacket zonepak(NPACKET_TYPE_SIMULTY_LAND_ZONE);

    zonepak << (NLINT16)type
            << (NLINT32)from.getX() << (NLINT32)from.getY()
            << (NLINT32)to.getX()   << (NLINT32)to.getY();

    net_client->packet_put(zonepak);


    std::cout << (NLINT16)type
              << (NLINT32)from.getX() << (NLINT32)from.getY()
              << (NLINT32)to.getX()   << (NLINT32)to.getY();

}


void Client::buy_building(Point where, int type) {

    NLPacket buildpak(NPACKET_TYPE_SIMULTY_BUILDING_BUILD);

    buildpak << (NLINT16)type
             << (NLINT32)where.getX() << (NLINT32)where.getY();

    std::cerr << "B: " << where << " - " << type << std::endl;

    net_client->packet_put(buildpak);

    gui->console_log("Built building (request)");
}

void Client::packet_handle(NLPacket p)
{
    switch(p.getType()) {

        case NPACKET_TYPE_SIMULTY_WELCOME:
        {
            std::cerr << "** Got welcome message" << std::endl;

            std::string welcome; p >> welcome;

            NLPacket ver(NPACKET_TYPE_SIMULTY_VERSION_CLIENT);
            ver << (NLINT16)0 << (NLINT16)0 << (NLINT16)1;
            net_client->packet_put(ver);

            alert("Welcome message from server", welcome.c_str(), NULL, "Ok", NULL, 0, 0);

            break;
        }

        case NPACKET_TYPE_SIMULTY_GAMEDATA: {
          std::string data;
          data = p.getData();
          //p.print();
          //std::cout << "The data is " << data.length() << " chars long and packet is " << p.getSize() << std::endl;
          LoaderSaver::loadGame(p.getData(), map, NULL, NULL);
          break;
        }

        case NPACKET_TYPE_SIMULTY_ID: {

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
        case NPACKET_TYPE_SIMULTY_ROAD_BUILD:
        {
            NLINT32 x, y; p >> x >> y;
            map->getTile(x, y)->setRoad(true);

            gui->console_log("Built road");
            break;
        }


        case NPACKET_TYPE_SIMULTY_TIME_INCR:
        {
            cal.advance();
            time++;
            break;
        }


        case NPACKET_TYPE_SIMULTY_MONEY_CHANGE:
        {
            NLINT16 player_affected; NLINT32 money_new;
            p >> player_affected >> money_new;

            gui->console_log("Money changed");

            break;
        }

        case NPACKET_TYPE_SIMULTY_LAND_BUY:
        {
            gui->console_log("Land is bought!");

            // Fetch area:
            NLINT16 pl_slot; NLINT32 x1, y1, x2, y2;
            p >> pl_slot >> x1 >> y1 >> x2 >> y2;

            for(int x = x1; x <= x2; x++)
                for(int y = y1; y <= y2; y++)
                {
                    if(map->getTile(x, y)->getOwner() == -1) {
                        map->getTile(x, y)->setOwner(pl_slot);
                    }
                }

            gui->console_log("Bought land");

            break;
        }

        case NPACKET_TYPE_SIMULTY_LAND_ZONE: {

            // Fetch area:
            NLINT16 pl_slot, ztype; NLINT32 x1, y1, x2, y2;
            p >> pl_slot >> ztype >> x1 >> y1 >> x2 >> y2;

            for(int x = x1; x <= x2; x++)
                for(int y = y1; y <= y2; y++)
                {
                    if(map->getTile(x, y)->getZone() == 0)
                        map->getTile(x, y)->setZone(ztype);
                }

            gui->console_log("Bought zone");

            break;

        }

        case NPACKET_TYPE_SIMULTY_PLAYER_JOINED:
        {
            std::string nick; NLINT32 id; NLINT16 slot;
            p >> id >> slot >> nick;


            gui->console_log("Player joined");

            break;

        } case NPACKET_TYPE_SIMULTY_PLAYER_LEFT: {
            NLINT32 id;
            p >> id;

            gui->console_log("Player left");

            break;

        } case NPACKET_TYPE_SIMULTY_BUILDING_BUILD: {

            NLINT16 buildingType, slot; NLINT32 x, y;
            p >> slot >> buildingType >> x >> y;

            std::cerr << "BC: " << buildingType << " " << Point(x, y) << std::endl;

            Building *b = BuildingFactory::getBuilding(buildingType, Point(x, y), slot);
            bman.addSpecialBuilding(b);

            gui->console_log("Built building");

            break;

        } default: {
            std::cerr << "** Got uknown message with id " << p.getType() << std::endl;
            p.print();
            break;
        }

    }


}

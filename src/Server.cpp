/*  _______ __                  __ __
 * |     __|__|.--------.--.--.|  |  |_.--.--.
 * |__     |  ||        |  |  ||  |   _|  |  |
 * |_______|__||__|__|__|_____||__|____|___  |
 *                                     |_____|
 * =====================================================================
 *
 * Simulty - The Open Source multiplayer city simulation game.
 *
 * Planning and code by: Gustav Tiger - admin@tigge.org
 *
 * The code and the art is released under the GNU Public License v2. A
 * copy can be found in the LICENCE.txt file shipped with the game.
 *
 * In addition the licence can be found at the followig address:
 * http://www.gnu.org/licenses/gpl.html
 *
 * =====================================================================
 *
 * Server.cpp - This file contains the main code for the server. It
 * keeps everything together.
 *
 * Version ##revision##
 */

#include "SimultyException.hpp"
#include "Server.hpp"

Server* Server::instance = NULL;

int main(int argc, char *argv[]) {

  try {
    Server *server = Server::getInstance();

    while(true)
    {
        server->update();
        rest(0);
    }

    delete server;

  } catch(std::exception &e) {
    std::cerr << "Simulty encountered an error which it couldn't recover from." << std::endl;
    std::cerr << "Error message: " << e.what() << std::endl;
  } catch(...) {
    std::cerr << "Simulty encountered an unkown error which it couldn't recover from." << std::endl;
  }
} END_OF_MAIN()


Server* Server::getInstance() {
  if (instance == NULL) {  
    instance = new Server;
  }
  return instance;
}

// Constructor, inits the server and starts listening after connections
// =====================================================================
Server::Server() {

  allegro_init();

  install_timer();

  net_server = net.add();
  net_server->listen_on(5557);


  std::cout << "  _______ __                  __ __         " << std::endl;
  std::cout << " |     __|__|.--------.--.--.|  |  |_.--.--." << std::endl;
  std::cout << " |__     |  ||        |  |  ||  |   _|  |  |" << std::endl;
  std::cout << " |_______|__||__|__|__|_____||__|____|___  |" << std::endl;
  std::cout << "                                     |_____|" << std::endl;
  std::cout << " Server - Released under GPL v2             " << std::endl;
  std::cout << " ___________________________________________" << std::endl;

  std::cerr << " * Starting game server                     " << std::endl;
  std::cerr << " * Initializing resources                   " << std::endl;
  std::cerr << " * Reticulating splines                     " << std::endl;

  std::cerr << std::endl;

  map = new Map(50, 50);

  time_advance = false;

  // Set initial speed:
  setSpeed(1);

  srand(time(0));
}

// Destructor, cleans everything up:
// =====================================================================
Server::~Server() {

    // TODO: clean up more things. playes, etc, etc

    // Delete map instance
    delete map;

}

// Timer called once every second: TODO, make static!
// =====================================================================
void Server::time_increment(void *object) {
    ((Server *)object)->time_advance = true;
}
END_OF_FUNCTION()


void Server::setSpeed(int speed) {
  if(speed >= 0 && speed <= 3) {
    this->speed = speed;
    if(speed > 0) {
      int msec = 0;
      switch(speed) {
        case 1: msec = 1000; break;
        case 2: msec = 500;  break;
        case 3: msec = 250;  break;
      }
      install_param_int_ex(Server::time_increment, this, MSEC_TO_TIMER(msec));
    } else {
      remove_param_int(Server::time_increment, this);
    }
  } else {
    throw SIMULTYEXCEPTION("The specified speed is not valid");
  }
}
int  Server::getSpeed() {
  return this->speed;
}

// Server update loop, the main loop:
// =====================================================================
void Server::update () {

  // Wait for network updates (max 250 ms, then move on)
  net.update(100);

  // New stuff happening: (TODO: move to function)
  if(time_advance) {

    if(calendar.isEndOfMonth()) {
      std::cout << "It is now " << calendar.getMonthAsString() << ", time to update zones" << std::endl;
      for(unsigned int i = 0; i < pman.count(); i++) {
        bman.updateZoneBuildings(pman.get_by_n(i)->getSlot(), map);
      }
    }

    if(calendar.isEndOfYear()) {
      // TODO: Make dependent of commerce and industry
      std::cout << "It is now " << calendar.getYear() << ", time for *ka-ching!*" << std::endl;
      for(unsigned int p = 0; p < pman.count(); p++) {
        int pop = 0;
        for(unsigned int i = 0; i < bman.getZoneBuildingCount(); i++) {
          if(bman.getZoneBuilding(i)->getOwner() == pman.get_by_n(p)->getSlot()) {
            pop += int(pow(bman.getZoneBuilding(i)->getWidth()*bman.getZoneBuilding(i)->getHeight(), 2.0)) *
            bman.getThriveValue(map, pman.get_by_n(p)->getSlot(), bman.getZoneBuilding(i)->getPosition());
          }
        }
        pman.get_by_n(p)->setMoney(pman.get_by_n(p)->getMoney() + pop*(1+pman.get_by_n(p)->getTax()/100));
      }
    }

    calendar.advance();

    for(unsigned int i = 0; i < pman.count(); i++) {
      player_server_network *plr = (player_server_network *)pman.get_by_n(i);

      if(plr->getType() == PLAYER_TYPE_SERVER_NETWORK) {
        NLPacket p(NLPACKET_TYPE_SIMULTY_TIME_INCR);
        plr->socket->packet_put(p);
      }
    }
    time_advance = false;
  }


  // New connection:
  // -----------------------------------------------------------------
  if(net_server->got_connection()) {
    player_add(PLAYER_TYPE_SERVER_NETWORK);
  }

  for(unsigned int i = 0; i < pman.count(); i++) {
    // Do only remote players (TODO: Better idea to make ai and remote players work kinda like the same
    if(pman.get_by_n(i)->getType() == PLAYER_TYPE_SERVER_NETWORK) {
      player_server_network *pl = (player_server_network *)pman.get_by_n(i);

      // A player have left or got disconnected somehow:
      if(pl->socket->is_connected() == false) {
        player_remove(pl);
      }

      // We got a packet:
      if(pl->socket->packet_exists()) {
        //err << "* Server have recieved a package, from client " << pl->socket->get_id() << "... " << endl;
        packet_handle(pl, pl->socket->packet_get());
      }
    }
  }
}


// PACKET HANDLING:
// =============================================================================
// =============================================================================

bool Server::packet_handle(player_server_network *from, NLPacket pack)
{
  switch(pack.getType())
  {
    // Version type:
    case NLPACKET_TYPE_SIMULTY_VERSION_CLIENT:
    {
        std::cerr << "** Got version info from client" << std::endl;


        NLPacket pgd(NLPACKET_TYPE_SIMULTY_GAMEDATA);
        std::string data = LoaderSaver::saveGame(map, NULL, NULL);
        pgd << data;
        //std::cout << "The data is " << data.length() << " chars long and packet is " << pgd.getSize() << std::endl;
        //pgd.print();
        from->socket->packet_put(pgd);

        NLPacket pid(NLPACKET_TYPE_SIMULTY_ID);
        pid << (NLINT32)from->getId() << (NLINT16)from->getSlot();

        from->socket->packet_put(pid);

        break;
    }

    // Bulldoze:
    case NLPACKET_TYPE_SIMULTY_REQUEST_BULLDOZE: {

      std::cout << "Bulldozing area..." << std::endl;
      
      Point fr = Point(pack.nextInt32(), pack.nextInt32());
      Point to = Point(pack.nextInt32(), pack.nextInt32());
      
      if(map->bulldozeCost(from->getSlot(), fr ,to) <= from->getMoney()) {
        map->bulldoze(from->getSlot(), fr, to);
        
        NLPacket packet(NLPACKET_TYPE_SIMULTY_BULLDOZE);
        packet << (NLINT16)from->getSlot() << (NLINT32)fr.getX() 
               << (NLINT32)fr.getY()       << (NLINT32)to.getX()
               << (NLINT32)to.getY();
               
        packet_send_to_all(packet);
      }

      // TODO: move to building manager
      /*
      for(unsigned int x = startX; x <= (unsigned int)endX && x < map->getWidth(); x++)
        for(unsigned int y = startY; y <= (unsigned int)endY && y < map->getHeight(); y++) {
          if(map->getTile(x, y)->getOwner() == from->getSlot()) {
            map->getTile(x, y)->setRoad(false);
            map->getTile(x, y)->setZone(0);
            if(bman.getSpecialBuildingID(Point(x, y)) != -1)
              bman.removeSpecialBuilding(bman.getSpecialBuildingID(Point(x, y)));
          }
        }
      */
      
      break;
    }

    // Buy land:
    case NLPACKET_TYPE_SIMULTY_REQUEST_LAND: {

      Point fr = Point(pack.nextInt32(), pack.nextInt32());
      Point to = Point(pack.nextInt32(), pack.nextInt32());

      if(map->buyLandCost(from->getSlot(), fr, to) <= from->getMoney()) {
        map->buyLand(from->getSlot(), fr, to);
        
        NLPacket packet(NLPACKET_TYPE_SIMULTY_LAND);
        packet << (NLINT16)from->getSlot() << (NLINT32)fr.getX() 
               << (NLINT32)fr.getY()       << (NLINT32)to.getX()
               << (NLINT32)to.getY();
               
        packet_send_to_all(packet);
      }

      break;
    }
    // Buy road:
    case NLPACKET_TYPE_SIMULTY_REQUEST_ROAD: {

      Point fr = Point(pack.nextInt32(), pack.nextInt32());
      Point to = Point(pack.nextInt32(), pack.nextInt32());

      if(map->buildRoadCost(from->getSlot(), fr, to) <= from->getMoney()) {
        map->buildRoad(from->getSlot(), fr, to);
        
        NLPacket packet(NLPACKET_TYPE_SIMULTY_ROAD);
        packet << (NLINT16)from->getSlot() << (NLINT32)fr.getX() 
               << (NLINT32)fr.getY()       << (NLINT32)to.getX()
               << (NLINT32)to.getY();
        packet_send_to_all(packet);
      }
      
      //packet_send_to_all(roadp);

      break;
    }

    case NLPACKET_TYPE_SIMULTY_REQUEST_ZONE:
    {
      NLINT32 tp = pack.nextInt16();
      Point fr   = Point(pack.nextInt32(), pack.nextInt32());
      Point to   = Point(pack.nextInt32(), pack.nextInt32());

      // TODO: move to Map

      std::cout << map->buildZoneCost(from->getSlot(), tp, fr, to) << std::endl;

      if(map->buildZoneCost(from->getSlot(), tp, fr, to) <= from->getMoney()) {
        map->buildZone(from->getSlot(), tp, fr, to);

        NLPacket zonepak(NLPACKET_TYPE_SIMULTY_ZONE);
        zonepak << (NLINT16)from->getSlot() << (NLINT16)tp 
                << (NLINT32)fr.getX() << (NLINT32)fr.getY() 
                << (NLINT32)to.getX() << (NLINT32)to.getY();

        packet_send_to_all(zonepak);
      }


    
    break;

    } case NLPACKET_TYPE_SIMULTY_REQUEST_SPECIAL_BUILDING: {
        NLINT16 buildingType; NLINT32 x, y;
        pack >> buildingType >> x >> y;

        if( x > 0 && y > 0) {
            std::cerr << "B: " << Point(x, y) << " - " << buildingType << std::endl;

            Building *b = BuildingFactory::getBuilding(buildingType, Point(x, y), from->getSlot());

            int cost;

            switch(buildingType)
            {
              case Building::TYPE_POLICE:
                cost = SIMULTY_COST_POLICE;
                break;
              case Building::TYPE_HOSPITAL:
                cost = SIMULTY_COST_HOSPITAL;
                break;
              case Building::TYPE_FIRE:
                cost = SIMULTY_COST_FIRE;
                break;
              default:
                cost = 0xffff; // Shouldn't end up here, but just in case - make 'em pay! (65k)
                break;
            }

            std::cerr << cost << std::endl;

            if( from->getMoney() >= cost && bman.canBuildSpecialBuilding(b, from->getSlot(), map)) {
              from->setMoney(from->getMoney() - cost);

              bman.addSpecialBuilding(b);

              NLPacket buildingPack(NLPACKET_TYPE_SIMULTY_SPECIAL_BUILDING);
              buildingPack << (NLINT16)from->getSlot() << buildingType << x << y;
              packet_send_to_all(buildingPack);

              std::cerr << "Building!" << std::endl;
            } else {
              delete b;
            }
        }

        break;

    } default: {
        std::cerr << "** Got uknown message with id "
                << NLPACKET_TYPE_SIMULTY_ZONE << " "
                << pack.getType() << std::endl;
        pack.print();

        return false;
    }
  }

  std::cerr << "Player has " << from->getMoney() << " money!" << std::endl;
  return true;
}


void Server::packet_send(player_server_network *to, NLPacket pack)
{
    if(to->getType() == PLAYER_TYPE_SERVER_NETWORK)
    {
        to->socket->packet_put(pack);
    }

}


void Server::packet_send_to_all(NLPacket pack)
{
    // Loop and send to all players:
    for(unsigned int i = 0; i < pman.count(); i++)
    {
        player_server_network *to = (player_server_network *)pman.get_by_n(i);
        if(to->getType() == PLAYER_TYPE_SERVER_NETWORK)
        {
            to->socket->packet_put(pack);
        }
    }

}

// PLAYER HANDLING:
// =============================================================================
// =============================================================================

// Player add, incomming from connection: (TODO, move to separate file)
// =====================================================================
bool Server::player_add(unsigned char type)
{
    // Handle new connection
    std::cerr << "* Player joined" << std::endl;


    // Get free slot:
    int slot = pman.slot_next();

    Player *player_tmp;

    // Player is an AI:
    if(type == PLAYER_TYPE_SERVER_AI)
    {
        player_server_ai *player_ai_tmp = new player_server_ai(0 - (rand() % 1024), slot);
        player_tmp = player_ai_tmp;

        std::cerr << "  - Player is an AI running on the server" << std::endl;

        // TODO: give player a negative id BETTER!
        // TODO: reject if full.

    }
    else if(type == PLAYER_TYPE_SERVER_NETWORK)
    {

        if(net_server->got_connection())
        {

            // Get a NSocket from the NNetwork object:
            NLSocket *socket_tmp = net.add(net_server);

            if(socket_tmp != NULL)
            {
                // Set up new player information:
                player_server_network *player_network_tmp = new player_server_network(socket_tmp->get_id(), slot);

                player_tmp = player_network_tmp;
                player_network_tmp->socket = socket_tmp;

                // Print som useful information:
                std::cerr << "  - Connected from " << player_network_tmp->socket->get_ip() << ":" << player_network_tmp->socket->get_port() << std::endl;

                // TODO SLOT, wich slot.. let the user decide?

                // TODO: REJECT IF ALREADY FULL AND STUFF!

                // Compose a welcome packet for our new player:
                NLPacket welcome;
                welcome.setType(NLPACKET_TYPE_SIMULTY_WELCOME);
                welcome << "Simulty";

                // Send the welcome packet:
                player_network_tmp->socket->packet_put(welcome);

                // welcome.print();

            }
            else
            {
                // TODO, delete tmp socket
                std::cerr << "* New connection failed!" << std::endl;
                return false;
            }
        }
        else
        {
            return false;
        }


    }

    // Write info
    std::cerr << "  - Gave player the id " << player_tmp->getId() << " and assigned to slot " << player_tmp->getSlot() << std::endl << std::endl;

    // Add player to the list
    pman.add(player_tmp);

    std::string nick = "player";
    NLPacket joined(NLPACKET_TYPE_SIMULTY_PLAYER_JOINED);
    joined << player_tmp->getId() << player_tmp->getSlot() << nick;

    packet_send_to_all(joined);

    return true;

}

// Player remove, removes disconnected player (TODO: move to separate file)
// =====================================================================
bool Server::player_remove(Player *pl)
{
    std::cerr << "* Player left" << std::endl;
    std::cerr << "  - Had the the id " << pl->getId() << " and was assigned to slot " << pl->getSlot() << std::endl;


    NLPacket left(NLPACKET_TYPE_SIMULTY_PLAYER_LEFT);
    left << pl->getId();

    packet_send_to_all(left);


    if(pman.del_by_p(pl))
    {
        std::cerr << "  - Deleted successfully" << std::endl;
        return true;
    }

    std::cerr << "  - Delete failed" << std::endl;
    return false;

}

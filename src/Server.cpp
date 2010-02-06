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

#include "Server.hpp"
#include "SimultyException.hpp"

Server* Server::instance = NULL;

int main(int argc, char *argv[]) {

  try {
    Server *server = Server::getInstance();

    while(true) {
        server->update();
        SDL_Delay(1);
    }

    delete server;

  } catch(std::exception &e) {
    std::cerr << "Simulty encountered an error which it couldn't recover from." << std::endl;
    std::cerr << "Error message: " << e.what() << std::endl;
  } catch(...) {
    std::cerr << "Simulty encountered an unkown error which it couldn't recover from." << std::endl;

  }
  return 0;
}

Server* Server::getInstance() {
  if (instance == NULL) {
    instance = new Server;
  }
  return instance;
}

// Constructor, inits the server and starts listening after connections
// =====================================================================
Server::Server() {

  SDL_Init(SDL_INIT_TIMER);
  
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
Uint32 Server::time_increment(Uint32 interval, void *object) {
    ((Server *)object)->time_advance = true;
    return interval;
}

void Server::setSpeed(int speed) {
  if(speed >= 0 && speed <= 3) {
    this->speed = speed;
    if(speed > 0) {
      int msec = 0;
      switch(speed) {
        case 1: msec = 700; break;
        case 2: msec = 400;  break;
        case 3: msec = 200;  break;
      }
      SDL_RemoveTimer(timerId);
      timerId = SDL_AddTimer(msec, Server::time_increment, this);
    } else {
      SDL_RemoveTimer(timerId);
    }
  } else {
    throw SIMULTYEXCEPTION("The specified speed is not valid");
  }
}
int  Server::getSpeed() {
  return this->speed;
}

Date Server::getDate() {
  return date;
}

// Server update loop, the main loop:
// =====================================================================
void Server::update () {

  // Wait for network updates (max 250 ms, then move on)
  net.update(100);

  // New stuff happening: (TODO: move to function)
  if(time_advance) {

    // Increase date:
    date.advance();

    // Do every new month:
    if(date.isEndOfMonth()) {
      std::cout << "It is now the end of " << date.getMonthAsString() << ", time to update zones" << std::endl;
      
      // Update budget
      for(unsigned int p = 0; p < pman.count(); p++) {

        Player *player = pman.get_by_n(p);

        // Move to PlayerHandlerServer ?
        int resIncome = 0, comIncome = 0, indIncome = 0;
        for(unsigned int i = 0; i < bman.getZoneBuildingCount(); i++) {
          if(bman.getZoneBuilding(i)->getOwner() == player->getSlot()) {
            resIncome += bman.getZoneBuilding(i)->getInhabitants();
            comIncome += bman.getZoneBuilding(i)->getStores();
            indIncome += bman.getZoneBuilding(i)->getJobs();
          }
        }
        
        int crimeExpense = 0, fireExpense = 0, healthExpense = 0;
        for(unsigned int i = 0; i < bman.getSpecialBuildingCount(); i++) {
          if(bman.getSpecialBuilding(i)->getOwner() == player->getSlot()) {
            int type = bman.getSpecialBuilding(i)->getType();
              
            if(type == Building::TYPE_POLICE)
              crimeExpense -= 100;
            else if(type == Building::TYPE_FIRE)
              fireExpense -= 100;
            else if(type == Building::TYPE_HOSPITAL)
              healthExpense -= 100;
          }
        }
        
        resIncome = (double)resIncome / 120.0 / (1.0 - player->getBudget()->getTax());
        comIncome = (double)comIncome / 120.0 / (1.0 - player->getBudget()->getTax());
        indIncome = (double)indIncome / 120.0 / (1.0 - player->getBudget()->getTax());
        
        int balance  = player->getBudget()->getBalance();

        updateBalance(player, date.getMonth(), 
            Budget::ITEM_INCOME_TAXES_RESIDENTAL, resIncome);
        updateBalance(player, date.getMonth(), 
            Budget::ITEM_INCOME_TAXES_COMMERSIAL, comIncome);
        updateBalance(player, date.getMonth(), 
            Budget::ITEM_INCOME_TAXES_INDUSTRIAL, indIncome);
        
        updateBalance(player, date.getMonth(), 
            Budget::ITEM_EXPENSE_CRIME, crimeExpense);
        updateBalance(player, date.getMonth(), 
            Budget::ITEM_EXPENSE_FIRE, fireExpense);
        updateBalance(player, date.getMonth(), 
            Budget::ITEM_EXPENSE_HEALTH, healthExpense);

      }
      
      // Update zones:
      for(unsigned int i = 0; i < pman.count(); i++) {
        bman.updateZoneBuildings(pman.get_by_n(i), map, date.getMonth());
      }
      
      

    }
    // Do every new year:
    if(date.isEndOfYear()) {
      std::cout << "It is now the end of" << date.getYear() << ", time for *ka-ching!*" << std::endl;
      // TODO:  fix budget stuff
    }

    // Update thrive values
    if(date.getDay() % 5 == 0) {
      updateThrive();
    }

    // Tell clients we have a time increase
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


void Server::updateThrive() {

  std::cout << " - Updating thrive values" << std::endl;

  for(unsigned int pi = 0; pi < pman.count(); pi++) {
  
    Player    *player = pman.get_by_n(pi);
    ThriveMap *tMap   = player->getThriveMap();
  
    std::cout << "   - Player: " << player->getSlot() << std::endl;

    for(unsigned int x = 0; x < map->getWidth(); x++) {
      for(unsigned int y = 0; y < map->getHeight(); y++) {

        Point point(x, y);

        // Update crime thrive values:
        // ---------------------------------------------------------------------
      
        // Without ANY police, returns -70. Yes, if the police station is 
        // farther, this lessens the value. But then maybe this would be the 
        // case in the real world. Say your store is being robbed, when would 
        // you complain the most, when there were no police, or when it took 
        // the police one hour to get there, and the theifs left 10 minutes ago?
        int distance = 100;
        
        for(unsigned int i = 0; i < bman.getSpecialBuildingCount(); i++) {
          if(bman.getSpecialBuilding(i)->getType() == Building::TYPE_POLICE
              && bman.getSpecialBuilding(i)->getOwner() == player->getSlot()) {
            
            int newDistance = Point::distance(Point(x, y),
                bman.getSpecialBuilding(i)->getPosition() + Point(
                (bman.getSpecialBuilding(i)->getWidth() - 1) / 2,
                (bman.getSpecialBuilding(i)->getHeight() - 1) / 2));
                
            if(distance == 0 || newDistance < distance) {
              distance = newDistance;
            }
          }
        }
        
        // You dont think: "Yay!! A police station, I wanna live there!!", but 
        // the area gets a bad reputation if there isn't one. Will only 
        // return < 1. (TODO: Good reputation (randomed statistics?))
        
        tMap->updateThrive(point, Thrive::TYPE_CRIME, (100 - distance) / 100);
        
        // Update connection thrive
        // ---------------------------------------------------------------------        
        if(map->isCloseToRoad(point))
          tMap->updateThrive(point, Thrive::TYPE_CONNECTIVITY, 1.0);
        else
          tMap->updateThrive(point, Thrive::TYPE_CONNECTIVITY, 0.0);
        
        // Update electricity thrive
        // ---------------------------------------------------------------------
        tMap->updateThrive(point, Thrive::TYPE_ELECTRICITY, 0.0);
        
        // Update taxes thrive
        // ---------------------------------------------------------------------
        double taxThrive = 1.0 - player->getBudget()->getTax();
        tMap->updateThrive(point, Thrive::TYPE_TAXES, taxThrive);
                
        // Update commerse thrive
        // ---------------------------------------------------------------------
        tMap->updateThrive(point, Thrive::TYPE_COMMERSE, 0.0);
        
        // Update work thrive
        // ---------------------------------------------------------------------
        tMap->updateThrive(point, Thrive::TYPE_WORKERS, 0.0);
        
        // Update jobs thrive
        // ---------------------------------------------------------------------
        
        // Not checking to ensure that buildings are connected to roads. They 
        // should not be built on if they arent, unless there is two "sub-nets".
        unsigned int jobs = 0;
        BuildingZone* closestStore = NULL;

        // Check for available jobs, and distance to closes store.
        for(unsigned int i = 0; i < bman.getZoneBuildingCount(); i++) {
        
          BuildingZone *b = bman.getZoneBuilding(i);
          if(b->getType() != Building::TYPE_RESIDENTIAL) {  // then you can get a job here
             // Checks at placement should prevent this from going out of bounds
            Point bCenter = b->getPosition();
            bCenter.translate(b->getWidth() / 2, b->getHeight() / 2);
            
            if(Point::distance(Point(x, y), bCenter) < 30) {
              // Employer nearby!
              // TODO: Move job functions to BuildingZone::

              if(b->getType() == Building::TYPE_INDUSTRIAL)
                jobs += b->getWidth()*b->getHeight()*10 - (unsigned int)pow(b->getLevel(), 2.0) + 5;
              else  // Commersial employer
                jobs += b->getWidth()*b->getHeight()*5 - (unsigned int)pow(b->getLevel(), 2.0) + 2;

              for(unsigned int i = 0; i < bman.getZoneBuildingCount(); i++) {
                BuildingZone *b = bman.getZoneBuilding(i);
                if(b->getType() == Building::TYPE_RESIDENTIAL) {  // then you can get a job here
                  if(Point::distance(Point(x, y),
                      Point(b->getPosition().getX() + b->getWidth(), b->getPosition().getY() + b->getHeight())) < 30) {

                    // TODO:
                    // What if these guys can reach jobs, which u cant reach from $where?
                    // Should we spread out jobs as good as they possibly can be?
                    // Maybe we need an citizen-class, if we want things to be really neat?
                    // Should all people work?
                    if(jobs < 0)
                    jobs -= b->getInhabitants();
                  }
                }
              }
            }
            /* Check for closes store 
            if(b->getType() == Building::TYPE_COMMERSIAL) {
              Point p = Point(b->getPosition().getX() + b->getWidth()/2, b->getPosition().getY() + b->getHeight()/2);

              if(closestStore != NULL || Point::distance(Point(x, y), p) 
                  < Point::distance(Point(x, y), closestStore->getPosition())) {
                 closestStore->setPosition(p);
              }
            } */
          }
        }

        // There might be pioneers who'd like to start up shop in your town
        // Check for available unbuilt zones.
        unsigned int pioneer_need = 0;
        for(unsigned int xp = x - 15; xp < x + 15; xp++) {
          for(unsigned int yp = y - 15; yp < y + 15; yp++) {
            if(!map->outOfBounds(Point(xp, yp))) {
              if(map->getTile(Point(xp, yp))->getZone() == SIMULTY_ZONE_COM
              || map->getTile(Point(xp, yp))->getZone() == SIMULTY_ZONE_IND)
                pioneer_need += 1;
            }
          }
        }

        // Calculate thrive
        int thrive = 0;

        // TODO: formulsas should be curves, not lines.
        if(closestStore != 0)
          // This could be an x²-curve, where the top is at origo, and the greater root will then decide when bad goes to worse.
          thrive += (200/Point::distance(Point(x, y), closestStore->getPosition()))*closestStore->getLevel();
        if(jobs == 0 && pioneer_need == 0)
          thrive -= 150;
        else if(jobs != 0)
          thrive += int(pow(200, 1/(jobs/2)));
        else
          thrive += pioneer_need; // nerf it!


        //return thrive;
        
        tMap->updateThrive(point, Thrive::TYPE_JOBS, 0.0);
        
        // Update environment thrive:
        // ---------------------------------------------------------------------
        
        // Calculate industrial noise. Other players' industry also counts ;)
        int noise = 0;
        for(unsigned int i = 0; i < bman.getZoneBuildingCount(); i++) {
          BuildingZone *b = bman.getZoneBuilding(i);
          if(b->getType() == Building::TYPE_INDUSTRIAL) {
            int d = Point::distance(Point(x, y), bman.getZoneBuilding(i)->getPosition());
            if(d < 10)
              noise += (10 - d) * bman.getZoneBuilding(i)->getLevel();
          }
        }
        // Small noise = High thrive, logical
        double noiseThrive = 1 - (sqrt(noise) / 25.0);
       
        tMap->updateThrive(point, Thrive::TYPE_ENVIRONMENT, noiseThrive);
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

      Point fr = Point::fromPacket(pack);
      Point to = Point::fromPacket(pack);

      // Cost to raise stuff on map
      int cost = map->bulldozeCost(from->getSlot(), fr ,to);
      cost += bman.bulldozeCost(from->getSlot(), map, fr, to);
      
      if(cost <= from->getBudget()->getBalance()) {
        map->bulldoze(from->getSlot(), fr, to);
        bman.bulldoze(from->getSlot(), map, fr, to);

        NLPacket packet(NLPACKET_TYPE_SIMULTY_BULLDOZE);
        packet << (NLINT16)from->getSlot() << (NLINT32)fr.getX()
               << (NLINT32)fr.getY()       << (NLINT32)to.getX()
               << (NLINT32)to.getY();

        packet_send_to_all(packet);

        bman.clearArea(map, fr, to);

        updateBalance(from, date.getMonth(), Budget::ITEM_EXPENSE_BUILD, -cost);
      }

      break;
    }

    case NLPACKET_TYPE_SIMULTY_REQUEST_DEZONE: {
      Point fr = Point::fromPacket(pack);
      Point to = Point::fromPacket(pack);
      
      int cost = map->deZoneCost(from->getSlot(), fr, to);
      map->deZone(from->getSlot(), fr, to);
      
      NLPacket packet(NLPACKET_TYPE_SIMULTY_DEZONE);
      packet << (NLINT16)from->getSlot() << (NLINT32)fr.getX()
             << (NLINT32)fr.getY()       << (NLINT32)to.getX()
             << (NLINT32)to.getY();
             
      packet_send_to_all(packet);
      
      updateBalance(from, date.getMonth(), Budget::ITEM_EXPENSE_BUILD, -cost);
      
      break;
    }

    // Buy land:
    case NLPACKET_TYPE_SIMULTY_REQUEST_LAND: {

      Point fr = Point::fromPacket(pack);
      Point to = Point::fromPacket(pack);

      int cost = map->buyLandCost(from->getSlot(), fr, to);
      if(cost <= from->getBudget()->getBalance()) {
        map->buyLand(from->getSlot(), fr, to);

        NLPacket packet(NLPACKET_TYPE_SIMULTY_LAND);
        packet << (NLINT16)from->getSlot() << (NLINT32)fr.getX()
               << (NLINT32)fr.getY()       << (NLINT32)to.getX()
               << (NLINT32)to.getY();

        packet_send_to_all(packet);
        
        updateBalance(from, date.getMonth(), Budget::ITEM_EXPENSE_BUILD, -cost);
      }

      break;
    }
    // Buy road:
    case NLPACKET_TYPE_SIMULTY_REQUEST_ROAD: {

      Point fr = Point::fromPacket(pack);
      Point to = Point::fromPacket(pack);

      std::cerr << "(" << fr.getX() << ", " << fr.getY() << ") -> (" << to.getX() << ", " << to.getY() << ")" << std::endl;
      int cost = map->buildRoadCost(from->getSlot(), fr, to);
      if(cost <= from->getBudget()->getBalance()) {
        map->buildRoad(from->getSlot(), fr, to);

        NLPacket packet(NLPACKET_TYPE_SIMULTY_ROAD);
        packet << (NLINT16)from->getSlot() << (NLINT32)fr.getX()
               << (NLINT32)fr.getY()       << (NLINT32)to.getX()
               << (NLINT32)to.getY();
        packet_send_to_all(packet);

        updateBalance(from, date.getMonth(), Budget::ITEM_EXPENSE_BUILD, -cost);
      }

      break;
    }

    // Buy zone:
    case NLPACKET_TYPE_SIMULTY_REQUEST_ZONE: {
      NLINT32 tp = pack.nextInt16();
      Point fr   = Point::fromPacket(pack);
      Point to   = Point::fromPacket(pack);

      int cost = map->buildZoneCost(from->getSlot(), tp, fr, to);

      if(cost <= from->getBudget()->getBalance()) {

        map->buildZone(from->getSlot(), tp, fr, to);

        NLPacket zonepak(NLPACKET_TYPE_SIMULTY_ZONE);
        zonepak << (NLINT16)from->getSlot() << (NLINT16)tp
                << (NLINT32)fr.getX() << (NLINT32)fr.getY()
                << (NLINT32)to.getX() << (NLINT32)to.getY();

        packet_send_to_all(zonepak);
        
        updateBalance(from, date.getMonth(), Budget::ITEM_EXPENSE_BUILD, -cost);
      }

      break;

    } case NLPACKET_TYPE_SIMULTY_REQUEST_SPECIAL_BUILDING: {
        NLINT16 buildingType; NLINT32 x, y;
        pack >> buildingType >> x >> y;

        if( x > 0 && y > 0) {
            std::cerr << "B: " << Point(x, y) << " - " << buildingType << std::endl;

            Building *b = BuildingFactory::getBuilding(buildingType,
                Point(x, y), from->getSlot(), date);

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
                throw SIMULTYEXCEPTION("Unkown building type");
                break;
            }

            std::cerr << cost << std::endl;
            if(bman.canBuildSpecialBuilding(b, from->getSlot(), map)) {
              std::cerr << "Space is good" << std::endl;
            } else {
              std::cerr << "Space is not good" << std::endl;
            }

            if(cost <= from->getBudget()->getBalance()  
                && bman.canBuildSpecialBuilding(b, from->getSlot(), map)) {

              bman.addSpecialBuilding(map, b);

              NLPacket packet(NLPACKET_TYPE_SIMULTY_BUILD_SPECIAL_BUILDING);
              packet << (NLINT16)from->getSlot() << buildingType << x << y;
              packet_send_to_all(packet);

              updateBalance(from, date.getMonth(), Budget::ITEM_EXPENSE_BUILD, -cost);
            } else {
              delete b;
            }
        }

        break;

    } case NLPACKET_TYPE_SIMULTY_REQUEST_DEBUG: {
      

      
      Point p = Point::fromPacket(pack);
      
      std::cout << " PRINTING THRIVE VALUES FOR " << p.getX() << ", " << p.getY() << std::endl;

      std::cout << " Total:  " <<  (double)from->getThriveMap()->getThrive(p) << std::endl;
      std::cout << " Crime:  " <<  from->getThriveMap()->getThrive(p, Thrive::TYPE_CRIME) << std::endl;
      std::cout << " Fire:   " <<  from->getThriveMap()->getThrive(p, Thrive::TYPE_FIRE) << std::endl;
      std::cout << " Health: " <<  from->getThriveMap()->getThrive(p, Thrive::TYPE_HEALTH) << std::endl;
      
      std::cout << " Elect.: " <<  from->getThriveMap()->getThrive(p, Thrive::TYPE_ELECTRICITY) << std::endl;
      std::cout << " Water:  " <<  from->getThriveMap()->getThrive(p, Thrive::TYPE_WATER) << std::endl;
      std::cout << " Env.:   " <<  from->getThriveMap()->getThrive(p, Thrive::TYPE_ENVIRONMENT) << std::endl;
      std::cout << " Con.:   " <<  from->getThriveMap()->getThrive(p, Thrive::TYPE_CONNECTIVITY) << std::endl;
      std::cout << " Taxes:  " <<  from->getThriveMap()->getThrive(p, Thrive::TYPE_TAXES) << std::endl;
      
      std::cout << " Jobs:   " <<  from->getThriveMap()->getThrive(p, Thrive::TYPE_JOBS) << std::endl;
      std::cout << " Com.:   " <<  from->getThriveMap()->getThrive(p, Thrive::TYPE_COMMERSE) << std::endl;
      std::cout << " Work:   " <<  from->getThriveMap()->getThrive(p, Thrive::TYPE_WORKERS) << std::endl;
    
      break;
    } default: {
      std::cerr << "** Got uknown message with id "
          << NLPACKET_TYPE_SIMULTY_ZONE << " " << pack.getType() << std::endl;
      pack.print();

      return false;
    }
  }

  std::cerr << "Player has " << from->getBudget()->getBalance() << " money!" << std::endl;
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
void Server::updateBalance(Player *p, unsigned char m, unsigned char t, int b) {

  if(b != 0) {

    // Update budget
    Budget *budget = p->getBudget();
    budget->updateBudgetItem(m, t, b);
    
    std::cout << "Updating budget. Item: " << (int)t << ", Month: " << (int)m << ", Balance: " << b << std::endl;
    std::cout << "Player count: " << pman.count() << std::endl;
    std::cout << "player %p - from arg  " << (int)p << std::endl;
    std::cout << "player %p - from pman " << (int)pman.get_by_n(0) << std::endl;
    std::cout << "Player id " << p->getId() << std::endl; 
    std::cout << "Player slot " << p->getSlot() << std::endl;
    // Tell about it
    NLPacket packet(NLPACKET_TYPE_SIMULTY_BUDGET_UPDATE);
    packet << (NLINT16)p->getSlot();
    packet << (NLINT16)m;
    packet << (NLINT16)t;
    packet << (NLINT32)b;
    packet_send_to_all(packet);
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
    int slot = pman.nextSlot();

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

    // Set up thrive map
    ThriveMap *tm = new ThriveMap(map->getWidth(), map->getHeight());
    player_tmp->setThriveMap(tm);

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

PlayerManagerServer *Server::getPlayerManager() {
  return &pman;
}

BuildingManagerServer *Server::getBuildingManager() {
  return &bman;
}

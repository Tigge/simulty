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


#include "shared.h"

#include "Server.hpp"
#include "elog.h"


#include "Player.hpp"
#include "player_server_network.h"
#include "player_server_ai.h"

#include "BuildingFactory.hpp"

#include "LoaderSaver.hpp"

Server *server;

int main(int argc, char *argv[])
{

    server = new Server;

    while(true)
    {
        server->update();
        rest(0);
    }


    delete server;

}
END_OF_MAIN()


// Constructor, inits the server and starts listening after connections
// =====================================================================
Server::Server() {

    allegro_init();

    install_timer();

    install_param_int_ex(Server::time_increment, this, BPS_TO_TIMER(1));

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
void Server::time_increment(void *object)
{
    ((Server *)object)->time_advance = true;
}
END_OF_FUNCTION()

// Server update loop, the main loop:
// =====================================================================
void Server::update (  ){

    // Wait for network updates (max 250 ms, then move on)
    net.update(50);


    // New stuff happening: (TODO: move to function)
    if(time_advance)
    {
        for(unsigned int i = 0; i < pman.count(); i++)
        {
            player_server_network *plr = (player_server_network *)pman.get_by_n(i);

            if(plr->type_get() == PLAYER_TYPE_SERVER_NETWORK)
            {
                NLPacket p(NPACKET_TYPE_SIMULTY_TIME_INCR);
                plr->socket->packet_put(p);
            }
        }
        time_advance = false;
    }


    // New connection:
    // -----------------------------------------------------------------
    if(net_server->got_connection())
    {
        player_add(PLAYER_TYPE_SERVER_NETWORK);
    }

    for(unsigned int i = 0; i < pman.count(); i++)
    {
        // Do only remote players (TODO: Better idea to make ai and remote players work kinda like the same
        if(pman.get_by_n(i)->type_get() == PLAYER_TYPE_SERVER_NETWORK)
        {
            player_server_network *pl = (player_server_network *)pman.get_by_n(i);

            // A player have left or got disconnected somehow:
            if(pl->socket->is_connected() == false)
            {
                player_remove(pl);

            }

            // We got a packet:
            if(pl->socket->packet_exists())
            {
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
        case NPACKET_TYPE_SIMULTY_VERSION_CLIENT: 
        {
            std::cerr << "** Got version info from client" << std::endl;
            
            
            NLPacket pgd(NPACKET_TYPE_SIMULTY_GAMEDATA);
            std::string data = LoaderSaver::saveGame(map, NULL, NULL);
            pgd << data;
            //std::cout << "The data is " << data.length() << " chars long and packet is " << pgd.getSize() << std::endl;
            //pgd.print();
            from->socket->packet_put(pgd);
            
            NLPacket pid(NPACKET_TYPE_SIMULTY_ID); 
            pid << (NLINT32)from->id_get() << (NLINT16)from->slot_get();

            from->socket->packet_put(pid);

            break;
        }
        
        // Buy land:
        case NPACKET_TYPE_SIMULTY_LAND_BUY:
        {
            std::cerr << "LAND!!" << std::endl;
        
            // Read values from packet:
            NLINT32 startx, starty, endx, endy;
            pack >> startx >> starty >> endx >> endy;   

            if(startx < 0 || starty < 0)break;

            // Calculate cost:                     
            int cost = 0;           
            
            for(int x = startx; x <= endx && x < map->getWidth(); x++)
                for(int y = starty; y <= endy && y < map->getHeight(); y++)
                    if(map->getTile(x, y)->getOwner() == -1)
                    {
                        cost += SIMULTY_COST_LAND;                                    
                    }

            // If player has enough money, do the change:
            if(from->money_get() > cost)
            {
                from->money_set(from->money_get() - cost);

                for(int x = startx; x <= endx && x < map->getWidth(); x++)
                    for(int y = starty; y <= endy && y < map->getHeight(); y++)
                        if(map->getTile(x, y)->getOwner() == -1) {
                            map->getTile(x, y)->setOwner(from->slot_get());
                        }


                // Send change to everyone else:

                NLPacket landp(NPACKET_TYPE_SIMULTY_LAND_BUY);
                landp << (NLINT16)from->slot_get() << startx << starty << endx << endy;

                packet_send_to_all(landp);
            }                 

            break;
        }
        // Buy road:
        case NPACKET_TYPE_SIMULTY_ROAD_BUILD:
        {
          NLINT32 fromX, fromY, toX, toY; NLPacket roadp = pack;
          roadp >> fromX >> fromY >> toX >> toY;

          std::cerr << "** Got road build command" << std::endl;
          
          int cost = 0;
          
          for(int x = fromX; x <= toX && x < map->getWidth(); x++) {
            for(int y = fromY; y <= toY && y < map->getHeight(); y++) {
            
              if(!map->getTile(x, y)->isRoad() 
                  && map->getTile(x, y)->getOwner() == from->slot_get()) {
                  
                  cost += SIMULTY_COST_ROAD;
                  
              }             
            }
          }
          if(from->money_get() > SIMULTY_COST_ROAD) {
            
            for(int x = fromX; x <= toX && x < map->getWidth(); x++) {
              for(int y = fromY; y <= toY && y < map->getHeight(); y++) {
              
                if(!map->getTile(x, y)->isRoad() 
                    && map->getTile(x, y)->getOwner() == from->slot_get()) {    

                  map->getTile(x, y)->setRoad(true);
                  
                  NLPacket p(NPACKET_TYPE_SIMULTY_ROAD_BUILD);
                  p << x << y;
                  packet_send_to_all(p);
                }
              }
            }        

            /*
            NLPacket mupd(NPACKET_TYPE_SIMULTY_MONEY_CHANGE);
            from->money -= 20; mupd << from->money;
            from->socket->packet_put(mupd);
            */
            
            from->money_set(from->money_get() - SIMULTY_COST_ROAD);

            //packet_send_to_all(pack);
          } 
          break;
        }
        
        case NPACKET_TYPE_SIMULTY_LAND_ZONE:
        {
            NLINT32 startx, starty, endx, endy; NLINT16 type;
            pack >> type >> startx >> starty >> endx >> endy;
            
            int cost = 0;
            
            for(int x = startx; x <= endx; x++)
                for(int y = starty; y <= endy; y++)
                    if(map->getTile(x, y)->getZone() == 0) {
                        cost += SIMULTY_COST_ZONE_IND;
                    }
                    
            if(cost < from->money_get()) {
                
                for(int x = startx; x <= endx; x++)
                    for(int y = starty; y <= endy; y++)
                        if(map->getTile(x, y)->getZone() == 0) {
                            map->getTile(x, y)->setZone(type);                            
                        }    
                        
                NLPacket zonepak(NPACKET_TYPE_SIMULTY_LAND_ZONE);
                zonepak << (NLINT16)from->slot_get() << type << startx << starty << endx << endy;

                packet_send_to_all(zonepak);
                
                std::cerr << "Zone: Built: " << std::endl;
            }
            else
            {
              std::cerr << "Zone: Didn't build: " << std::endl;
            }
            
            break;
            
        } case NPACKET_TYPE_SIMULTY_BUILDING_BUILD: {
                    
            NLINT16 buildingType; NLINT32 x, y;            
            pack >> buildingType >> x >> y;
        
            std::cerr << "B: " << Point(x, y) << " - " << buildingType << std::endl;
        
            Building *b = BuildingFactory::getBuilding(buildingType, Point(x, y), from->slot_get());
            
            if(bman.canBuildSpecialBuilding(b, Point(x, y), from->slot_get(), map)) {
              bman.addSpecialBuilding(b);         
              
              NLPacket buildingPack(NPACKET_TYPE_SIMULTY_BUILDING_BUILD);            
              buildingPack << (NLINT16)from->slot_get() << buildingType << x << y;            
              packet_send_to_all(buildingPack);           
              
              std::cerr << "Building!" << std::endl;                          
            } else {
              delete b;
            }
                        
            break;
        
        } default: {
            std::cerr << "** Got uknown message with id " 
                    << NPACKET_TYPE_SIMULTY_LAND_ZONE << " " 
                    << pack.getType() << std::endl;
            pack.print();
            
            return false;
        }   
            
        
    }
    return true;
}


void Server::packet_send(player_server_network *to, NLPacket pack)
{
    if(to->type_get() == PLAYER_TYPE_SERVER_NETWORK)
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
        if(to->type_get() == PLAYER_TYPE_SERVER_NETWORK)
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
                welcome.setType(NPACKET_TYPE_SIMULTY_WELCOME);
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
    std::cerr << "  - Gave player the id " << player_tmp->id_get() << " and assigned to slot " << player_tmp->slot_get() << std::endl << std::endl;

    // Add player to the list
    pman.add(player_tmp);
    
    std::string nick = "player";
    NLPacket joined(NPACKET_TYPE_SIMULTY_PLAYER_JOINED);
    joined << player_tmp->id_get() << player_tmp->slot_get() << nick;
    
    packet_send_to_all(joined);    

    return true;

}

// Player remove, removes disconnected player (TODO: move to separate file)
// =====================================================================
bool Server::player_remove(Player *pl)
{
    std::cerr << "* Player left" << std::endl;
    std::cerr << "  - Had the the id " << pl->id_get() << " and was assigned to slot " << pl->slot_get() << std::endl;


    NLPacket left(NPACKET_TYPE_SIMULTY_PLAYER_LEFT);
    left << pl->id_get();
    
    packet_send_to_all(left);


    if(pman.del_by_p(pl))
    {
        std::cerr << "  - Deleted successfully" << std::endl;  
        return true;          
    }

    std::cerr << "  - Delete failed" << std::endl;
    return false;
    
}

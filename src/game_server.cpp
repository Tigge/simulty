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
 * game_server.cpp - This file contains the main code for the server. It
 * keeps everything together. 
 *
 * Version ##revision##
 */


#include "allegro.h"

#include "shared.h"

#include "game_server.h"
#include "elog.h"


#include "player.h"
#include "player_server_network.h"
#include "player_server_ai.h"


// Constructor, inits the server and starts listening after connections
// =====================================================================
game_server::game_server() {

    allegro_init();

    install_timer();

    install_param_int_ex(game_server::time_increment, this, BPS_TO_TIMER(1));

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

    m = new map_base;

    time_advance = false;
    

}

// Destructor, cleans everything up:
// =====================================================================
game_server::~game_server() {


    // TODO: clean up more things. playes, etc, etc

    // Delete map instance
    delete m;

}

// Timer called once every second: TODO, make static!
// =====================================================================
void game_server::time_increment(void *object)
{
    ((game_server *)object)->time_advance = true;
}
END_OF_FUNCTION()

// Server update loop, the main loop:
// =====================================================================
void game_server::update (  ){

    // Wait for network updates (max 250 ms, then move on)
    net.update(250);


    // New stuff happening: (TODO: move to function)
    if(time_advance)
    {
        for(unsigned int i = 0; i < pman.count(); i++)
        {
            player_server_network *plr = (player_server_network *)pman.get_by_n(i);

            if(plr->type_get() == PLAYER_TYPE_SERVER_NETWORK)
            {
                NPacket p(NPACKET_TYPE_SIMULTY_TIME_INCR);
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


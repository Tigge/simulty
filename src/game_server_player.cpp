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
 * game_server.cpp - This file contains player handling codes for adding
 * and removing players.
 *
 * Version ##revision##
 */

#include "shared.h"

#include "game_server.h"

#include "player.h"
#include "player_server_network.h"
#include "player_server_ai.h"

// Player add, incomming from connection: (TODO, move to separate file)
// =====================================================================
bool game_server::player_add(unsigned char type)
{
    // Handle new connection
    err << "* Player joined" << endl;
    

    // Get free slot:    
    int slot = pman.slot_next();
   
    player *player_tmp;

    // Player is an AI:
    if(type == PLAYER_TYPE_SERVER_AI)
    {
        player_server_ai *player_ai_tmp = new player_server_ai(0 - (rand() % 1024), slot);      
        player_tmp = player_ai_tmp;

        err << "  - Player is an AI running on the server" << endl;

        // TODO: give player a negative id BETTER!
        // TODO: reject if full. 
        
    }
    else if(type == PLAYER_TYPE_SERVER_NETWORK)
    {

        if(net_server->got_connection())
        {

            // Get a NSocket from the NNetwork object:
            NSocket *socket_tmp = net.add(net_server);

            if(socket_tmp != NULL)
            {
                // Set up new player information:
                player_server_network *player_network_tmp = new player_server_network(socket_tmp->get_id(), slot);

                player_tmp = player_network_tmp;
                player_network_tmp->socket = socket_tmp;

                // Print som useful information:
                err << "  - Connected from " << player_network_tmp->socket->get_ip() << ":" << player_network_tmp->socket->get_port() << endl;

                // TODO SLOT, wich slot.. let the user decide?

                // TODO: REJECT IF ALREADY FULL AND STUFF!

                // Compose a welcome packet for our new player:
                NPacket welcome;
                welcome.setType(NPACKET_TYPE_SIMULTY_WELCOME);
                welcome << "Simulty";

                // Send the welcome packet:
                player_network_tmp->socket->packet_put(welcome);

                // welcome.print();

            }
            else 
            {
                // TODO, delete tmp socket
                err << "* New connection failed!" << endl;
                return false;
            }
        }
        else
        {
            return false;
        }
        
        
    }
    
    // Write info
    err << "  - Gave player the id " << player_tmp->id_get() << " and assigned to slot " << player_tmp->slot_get() << endl << endl;

    // Add player to the list
    pman.add(player_tmp);
    
    std::string nick = "player";
    NPacket joined(NPACKET_TYPE_SIMULTY_PLAYER_JOINED);
    joined << player_tmp->id_get() << player_tmp->slot_get() << nick;
    
    packet_send_to_all(joined);    

    return true;

}

// Player remove, removes disconnected player (TODO: move to separate file)
// =====================================================================
bool game_server::player_remove(player *pl)
{
    err << "* Player left" << endl;
    err << "  - Had the the id " << pl->id_get() << " and was assigned to slot " << pl->slot_get() << endl;


    NPacket left(NPACKET_TYPE_SIMULTY_PLAYER_LEFT);
    left << pl->id_get();
    
    packet_send_to_all(left);


    if(pman.del_by_p(pl))
    {
        err << "  - Deleted successfully" << endl;  
        return true;          
    }

    err << "  - Delete failed" << endl;
    return false;
    
}

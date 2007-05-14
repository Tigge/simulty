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
//#include "allegro.h"

#include "client.h"




#include "shared.h"

#include "game_server.h"
#include "elog.h"


#include "player.h"
#include "player_server_network.h"
#include "player_server_ai.h"

bool game_server::packet_handle(player_server_network *from, NPacket pack)
{

    switch(pack.getType())
    {
        // Version type:
        case NPACKET_TYPE_SIMULTY_VERSION_CLIENT: 
        {
            std::cerr << "** Got version info from client" << std::endl;
            NPacket pid(NPACKET_TYPE_SIMULTY_ID); 
            pid << (INT32)from->id_get() << (INT16)from->slot_get();

            from->socket->packet_put(pid);

            break;
        }
        
        // Buy land:
        case NPACKET_TYPE_SIMULTY_LAND_BUY:
        {
            // Read values from packet:
            INT32 startx, starty, endx, endy;
            pack >> startx >> starty >> endx >> endy;   

            // Calculate cost:                     
            int cost = 0;
            
            for(int x = startx; x <= endx; x++)
                for(int y = starty; y <= endy; y++)
                    if(m->tiles[y * m->width + x].owner == -1)
                    {
                        cost += SIMULTY_COST_LAND;                                    
                    }

            // If player has enough money, do the change:
            if(from->money_get() > cost)
            {
                from->money_set(from->money_get() - cost);

                for(int x = startx; x <= endx; x++)
                    for(int y = starty; y <= endy; y++)
                        if(m->tiles[y * m->width + x].owner == -1)
                        {
                            m->tiles[y * m->width + x].owner = from->slot_get();                                 
                        }


                // Send change to everyone else:

                NPacket landp(NPACKET_TYPE_SIMULTY_LAND_BUY);
                landp << (INT16)from->slot_get() << startx << starty << endx << endy;

                packet_send_to_all(landp);
            }                 

            break;
        }
        // Buy road:
        case NPACKET_TYPE_SIMULTY_ROAD_BUILD:
        {
            INT32 roadx; INT32 roady; NPacket roadp = pack;
            roadp >> roadx >> roady;

            std::cerr << "** Got road build command" << std::endl;
            if(from->money_get() > SIMULTY_COST_ROAD && !m->tiles[roady * m->width + roadx].road && 
                m->tiles[roady * m->width + roadx].owner == from->slot_get())
            {
                m->tiles[roady * m->width + roadx].road = true;
                /*
                NPacket mupd(NPACKET_TYPE_SIMULTY_MONEY_CHANGE);
                from->money -= 20; mupd << from->money;
                from->socket->packet_put(mupd);
                */
                
                from->money_set(from->money_get() - SIMULTY_COST_ROAD);

                packet_send_to_all(pack);
            }
            else
            {
                std::cerr << "Road: Didn't build: money: " << from->money_get() << " owner: " << (int)(m->tiles[roady * m->width + roadx].owner) << " road: " << (int)(m->tiles[roady * m->width + roadx].road) << std::endl;
            }
            break;
        }
        
        case NPACKET_TYPE_SIMULTY_LAND_ZONE:
        {
            INT32 startx, starty, endx, endy; INT16 type;
            pack >> type >> startx >> starty >> endx >> endy;
            
            int cost = 0;
            
            for(int x = startx; x <= endx; x++)
                for(int y = starty; y <= endy; y++)
                    if(m->tiles[y * m->width + x].zone == 0)
                    {
                        cost += SIMULTY_COST_ZONE_IND;
                    }
                    
            if(cost < from->money_get()) {
                
                for(int x = startx; x <= endx; x++)
                    for(int y = starty; y <= endy; y++)
                        if(m->tiles[y * m->width + x].zone == 0)
                        {
                            m->tiles[y * m->width + x].zone = type;
                            
                            
                        }    
                        
                NPacket zonepak(NPACKET_TYPE_SIMULTY_LAND_ZONE);
                zonepak << (INT16)from->slot_get() << type << startx << starty << endx << endy;

                packet_send_to_all(zonepak);
                
                std::cerr << "Zone: Built: " << std::endl;
            }
            else
            {
              std::cerr << "Zone: Didn't build: " << std::endl;
            }
            
            
            
            break;
        }


        default:
        {
            std::cerr << "** Got uknown message with id " << NPACKET_TYPE_SIMULTY_LAND_ZONE << " " << pack.getType() << std::endl;
            pack.print();
            
            return false;
        }   
            
        
    }
    return true;
}


void game_server::packet_send(player_server_network *to, NPacket pack)
{
    if(to->type_get() == PLAYER_TYPE_SERVER_NETWORK)
    {
        to->socket->packet_put(pack);
    }

}


void game_server::packet_send_to_all(NPacket pack)
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







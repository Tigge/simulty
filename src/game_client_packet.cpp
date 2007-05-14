#include "allegro.h"


#include "client.h"
#include "shared.h"
#include "game_client.h"



void game_client::packet_handle(NPacket p)
{
    switch(p.getType())
        {
            case NPACKET_TYPE_SIMULTY_WELCOME: 
            {
                std::cerr << "** Got welcome message" << std::endl;

                std::string welcome; p >> welcome;

                NPacket ver(NPACKET_TYPE_SIMULTY_VERSION_CLIENT); 
                ver << (INT16)0 << (INT16)0 << (INT16)1;
                net_client->packet_put(ver);

                alert("Welcome message from server", welcome.c_str(), NULL, "Ok", NULL, 0, 0);

                break;
            }

            case NPACKET_TYPE_SIMULTY_ID:
            {
                std::cerr << "** Got id message (update)" << std::endl;

                // Fetch ID and slot from packet:
                INT32 id_new; INT16 slot_new; p >> id_new >> slot_new;
                
                // Create new local player and assign id and slot:
                player_client_local *pl = new player_client_local(id_new, slot_new);
                //pl->get_id() = id_new; pl->slot = slot_new;

                // Add to player manager:
                pman.add(pl);

                // Set the player to our local player:                
                player_me = pl;
       
                std::cerr << "*** ID is " << id_new << " and slot is " << slot_new << std::endl;
                
                break;
            }
            case NPACKET_TYPE_SIMULTY_ROAD_BUILD:
            {
                INT32 x, y; p >> x >> y;
                m->tiles[y * m->width + x].road = true;
                break;
            }


            case NPACKET_TYPE_SIMULTY_TIME_INCR:
            {
                time++;
                break;
            }


            case NPACKET_TYPE_SIMULTY_MONEY_CHANGE:
            {
                INT16 player_affected; INT32 money_new;
                p >> player_affected >> money_new;
                
                
                break;
            }

            case NPACKET_TYPE_SIMULTY_LAND_BUY:
            {
                // Fetch area:
                INT16 pl_slot; INT32 x1, y1, x2, y2;
                p >> pl_slot >> x1 >> y1 >> x2 >> y2;
            
                for(int x = x1; x <= x2; x++)
                    for(int y = y1; y <= y2; y++)
                    {
                        if(m->tiles[y * m->width + x].owner == -1)
                            m->tiles[y * m->width + x].owner = pl_slot;
                    }
                break;
            }
            
            case NPACKET_TYPE_SIMULTY_LAND_ZONE:
            {
                // Fetch area:
                INT16 pl_slot, ztype; INT32 x1, y1, x2, y2;
                p >> pl_slot >> ztype >> x1 >> y1 >> x2 >> y2;
            
                for(int x = x1; x <= x2; x++)
                    for(int y = y1; y <= y2; y++)
                    {
                        if(m->tiles[y * m->width + x].zone == 0)
                            m->tiles[y * m->width + x].zone = ztype;
                    }
                    
                client->g->console_log("Bought zone");
                    
                break;            
            
            }
            
            case NPACKET_TYPE_SIMULTY_PLAYER_JOINED:
            {
                std::string nick; INT32 id; INT16 slot;
                p >> id >> slot >> nick;
                
                
                client->g->console_log("Player joined");                
            
                break;
            }
            
            case NPACKET_TYPE_SIMULTY_PLAYER_LEFT:
            {
                INT32 id;
                p >> id;
                
                client->g->console_log("Player left");
                            
                break;
            }

            default:
            {
                std::cerr << "** Got uknown message with id " << p.getType() << std::endl;
                p.print();
                break;
            }           

        }












}

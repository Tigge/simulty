
#include "shared.h"

#include "Client.hpp"

#define err std::cerr

Client *client;


int main(int argc, char *argv[])
{
    std::cerr << "Starting..." << std::endl;    
    client = new Client;
    std::cerr << "New game client" << std::endl;

    while(client->running())
    {
        //cerr << ".";
        while(client->needupdate())
        {
            client->update();
        }

        client->render();
        rest(0);
    }

    std::cerr << "Deleting game client..." << std::endl;
    delete client;

    std::cerr << "Ending..." << std::endl;
    
    return 0;
}
END_OF_MAIN()



// TODO - static functions instead?
void speedhandler(void *data)
{
    ((Client *)data)->speed_counter++;
}
END_OF_FUNCTION()

void fpshandler(void *data)
{
    ((Client *)data)->fps = ((Client *)data)->frames;
    ((Client *)data)->frames = 0;
}
END_OF_FUNCTION()

void mousehandler(int flags)
{

}


Client::Client()
{

    // Initializing allegro (and some sub elements)
    allegro_init();

    install_keyboard();
    install_timer();
    install_mouse();

    // Locking variables and functions
    LOCK_VARIABLE(fps);
    LOCK_VARIABLE(speed_counter);
    LOCK_VARIABLE(frames);

    LOCK_FUNCTION(speedhandler);
    LOCK_FUNCTION(fpshandler);

    // Setting color depth
    std::cout << "Allegro inited..." << std::endl;

    set_color_depth(16);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
    set_color_conversion(COLORCONV_TOTAL | COLORCONV_KEEP_TRANS);

    // Create double buffer
    buffer = create_bitmap(800, 600);    
    


    if(!buffer)
    {
        allegro_message("Couldn't load / create some images");
        exit(1);
    }

    // Add local socket and connect it to server (TODO: move later)
    net_client = net.add();
    net_client->connect_to("home.tigge.org", 5557);

    install_param_int_ex(speedhandler, this, BPS_TO_TIMER(60));
    install_param_int_ex(fpshandler, this, BPS_TO_TIMER(1));

    fps =  speed_counter =  frames = 0;

    state_running = true;
    state_menu  = true;
    state_game = false;

    map = new GUIMap(30, 30);
    gui = new GUI;

    time = 0;



    // Set max players and set up all slots:

    player_me = NULL;

}

Client::~Client (){



}

bool Client::needupdate()
{
    return speed_counter > 0;
}

bool Client::running()
{
    return state_running;
}

void Client::render ()
{
    // Clear the double buffer:
    clear_bitmap(buffer);

    if(state_game) {
        
        if(state_game == SIMULTY_CLIENT_STATE_GAME_ON) {

            //m->render(buffer, cam);

        } 
    }

    // Render gui:
    gui->render(buffer);

    // Render buffer to screen:
    blit(buffer, screen, 0, 0, 0, 0, 800, 600);

    // Increase number of frames:
    frames++;
}



void Client::update (  )
{

    speed_counter--;  

    // Update gui
    gui->update();
    
    
    if(state_menu) {
    
        if(mouse_b & 1) { state_menu = false; state_game = SIMULTY_CLIENT_STATE_GAME_ON; }
        
    } else if(state_game) {
    
        net.update(0);

        if(net_client->packet_exists())
        {
            //err << "* Client have recieved a package... " << endl;
            packet_handle(net_client->packet_get());            

        }        
     }
}


void Client::buy_land(Point from, Point to) {

    client->gui->console_log("Bought land (request)");

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
  
    client->gui->console_log("Bought zone (request)");

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
        
    buildpak << (NLINT16)Building::TYPE_POLICE 
             << (NLINT32)where.getX() << (NLINT32)where.getY();

    std::cerr << "B: " << where << " - " << type << std::endl;

    net_client->packet_put(buildpak); 

    client->gui->console_log("Built building (request)");
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

        case NPACKET_TYPE_SIMULTY_ID:
        {
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
    
            std::cerr << "*** ID is " << id_new << " and slot is " << slot_new << std::endl;
            
            break;
        }
        case NPACKET_TYPE_SIMULTY_ROAD_BUILD:
        {
            NLINT32 x, y; p >> x >> y;
            map->getTile(x, y)->setRoad(true);
            
            client->gui->console_log("Built road");
            break;
        }


        case NPACKET_TYPE_SIMULTY_TIME_INCR:
        {
            time++;
            break;
        }


        case NPACKET_TYPE_SIMULTY_MONEY_CHANGE:
        {
            NLINT16 player_affected; NLINT32 money_new;
            p >> player_affected >> money_new;
            
            client->gui->console_log("Money changed");
            
            break;
        }

        case NPACKET_TYPE_SIMULTY_LAND_BUY:
        {
            client->gui->console_log("Land is bought!");
            
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
                
            client->gui->console_log("Bought land");
            
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
                
            client->gui->console_log("Bought zone");
                
            break;            
        
        }
        
        case NPACKET_TYPE_SIMULTY_PLAYER_JOINED:
        {
            std::string nick; NLINT32 id; NLINT16 slot;
            p >> id >> slot >> nick;
            
            
            client->gui->console_log("Player joined");                
        
            break;
            
        } case NPACKET_TYPE_SIMULTY_PLAYER_LEFT: {
            NLINT32 id;
            p >> id;
            
            client->gui->console_log("Player left");
                        
            break;
            
        } case NPACKET_TYPE_SIMULTY_BUILDING_BUILD: {
                    
            NLINT16 buildingType, slot; NLINT32 x, y;            
            p >> slot >> buildingType >> x >> y;
        
            std::cerr << "BC: " << buildingType << " " << Point(x, y) << std::endl;
        
            Building *b = BuildingFactory::getBuilding(buildingType, Point(x, y), slot);
            bman.addSpecialBuilding(b);
           
            client->gui->console_log("Built building");
            
            break;
            
        } default: {
            std::cerr << "** Got uknown message with id " << p.getType() << std::endl;
            p.print();
            break;
        }           

    }


}

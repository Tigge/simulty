
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

    m = new map;
    g = new gui;

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
    g->render(buffer);

    // Render buffer to screen:
    blit(buffer, screen, 0, 0, 0, 0, 800, 600);

    // Increase number of frames:
    frames++;
}



void Client::update (  )
{

    speed_counter--;  

    // Update gui
    g->update();
    
    
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

    std::cout << "buy from " 
              << from.getX() << ", " << from.getY() << " to "
              << to.getX() << ", "   << to.getY() << std::endl;

    NLPacket landpak(NPACKET_TYPE_SIMULTY_LAND_BUY);
    landpak << (NLINT32)from.getX() << (NLINT32)from.getY() 
            << (NLINT32)to.getX()   << (NLINT32)to.getY();
            
    net_client->packet_put(landpak);
}

void Client::buy_road(Point from, Point to) {

    if(!m->get(to.getX(), to.getY()).road)
    {
        NLPacket roadpak(NPACKET_TYPE_SIMULTY_ROAD_BUILD);
        roadpak << (NLINT32)to.getX() << (NLINT32)to.getY();
        net_client->packet_put(roadpak);
    }
}

void Client::buy_zone(Point from, Point to, int type) {

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

}

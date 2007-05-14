#include "allegro.h"
#include "client.h"
#include "game_client.h"
#include <iostream>


game_client *client;


int main(int argc, char *argv[])
{
    std::cerr << "Starting..." << std::endl;    
    client = new game_client;
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
}
END_OF_MAIN()

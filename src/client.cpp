#include "allegro.h"
#include "client.h"
#include "game_client.h"
#include <iostream>


game_client *client;


int main(int argc, char *argv[])
{
    cerr << "Starting..." << endl;    
    client = new game_client;
    cerr << "New game client" << endl;

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

    cerr << "Deleting game client..." << endl;
    delete client;

    cerr << "Ending..." << endl;
}
END_OF_MAIN()


#include "game_server.h"

#include "netlib.h"


game_server *server;

int main(int argc, char *argv[])
{

    server = new game_server;

    while(true)
    {
        server->update();
        rest(0);
    }


    delete server;

}
END_OF_MAIN()

#include "ServerListener.hpp"


void ServerListener::run() {
    while(true) {
        std::cout << "ServerListener: Accepting connections..." << std::endl;
        NL::Socket *socket = serverSocket->acceptConnection();
        std::cout << "ServerListener: Got a connection..." << std::endl;

        // TODO: Let main thread know about the new client. Preferably with
        //       the SDL event system.
        
        // TODO: big time
        SDL_Event event;
        event.type = SDL_USEREVENT;
        event.user.code  = USEREVENT_GOTPLAYER;
        event.user.data1 = socket;
        event.user.data2 = NULL;
        SDL_PushEvent(&event);
    } 

}

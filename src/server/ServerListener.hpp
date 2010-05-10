#ifndef _SERVERLISTENER_HPP_
#define _SERVERLISTENER_HPP_

#include "shared.h"
#include "NL.hpp"
#include "SDL.h"

#include "Runnable.hpp"

class ServerListener: public Runnable {

    private:

    NL::ServerSocket* serverSocket;
    
    public:
    
    ServerListener(int port) {
        serverSocket = new NL::ServerSocket();
        serverSocket->bindPort(port);
    }
    
    ~ServerListener() {
        delete serverSocket;    
    }
    
    virtual void run();

};

#endif


#include "ServerSocket.hpp"

#include <cstring>

namespace NL {

    ServerSocket::ServerSocket() {

    }

    void ServerSocket::bindPort(int port) {
        bindPort(port, DEFAULT_BACKLOG);
    }

    void ServerSocket::bindPort(int port, int backlog) {

        struct sockaddr_in address;

        address.sin_family      = AF_INET;
        address.sin_port        = htons(port);
        address.sin_addr.s_addr = INADDR_ANY;
        memset(&address.sin_zero, 0, sizeof(address.sin_zero));
        
        if(bind(this->getSocketId(), (struct sockaddr *)&address, 
                sizeof(struct sockaddr_in)) == -1)
        {
            DEBUG(std::cout << "ServerSocket: Listen, bind on socket " 
                    << this->getSocketId() << " failed - " 
                    << errno << ": " << strerror(errno) << std::endl;)
            throw Exception();
        }

        DEBUG(std::cout << "ServerSocket: Listen on, bind done" 
                << std::endl;)

        if(listen(this->getSocketId(), backlog) == -1)
        {
            DEBUG(std::cout << "ServerSocket: Listen, listen on socket " 
                    << this->getSocketId() << " failed" << std::endl;)
            throw Exception();
        }

        DEBUG(std::cout << "ServerSocket: Listen on, listen done" 
                << std::endl;)
     
        this->ipAddress = "127.0.0.1";
        this->port      = port;
    }

    Socket *ServerSocket::acceptConnection() {
    
        int remoteSocketId = accept(getSocketId(), NULL, NULL);
        if(remoteSocketId == -1) {
            DEBUG(std::cout << "ServerSocket: add (incomming connection), "
                    << "failed (" << remoteSocketId << ", " 
                    << errno << "):" << strerror(errno) << std::endl;)
            throw Exception();
        }

        return new Socket(remoteSocketId);
    }

}

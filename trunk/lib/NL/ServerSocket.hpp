#ifndef _NLSERVERSOCKET_HPP_
#define _NLSERVERSOCKET_HPP_

#include "Shared.hpp"
#include "AbstractSocket.hpp"
#include "Socket.hpp"

#include <vector>
#include <string>
#include <iostream>

namespace NL {

    class ServerSocket: public AbstractSocket {

        private:

        friend class Network;
        
        static const int DEFAULT_BACKLOG = 10;
        
        public:
        
        ServerSocket();
        
        void bindPort(int port);
        void bindPort(int port, int backlog);
        
        Socket *acceptConnection();
        
    };
    
}

#endif


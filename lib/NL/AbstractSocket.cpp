#include "AbstractSocket.hpp"

namespace NL {

    AbstractSocket::AbstractSocket() {

        int socketId = socket(PF_INET, SOCK_STREAM, 0);
        if(socketId == -1)
        {
            DEBUG(std::cout << "AbstractSocket: Failed to initialize " 
                    << "socket - " << h_errno << std::endl;)
            throw Exception();
        }
        
        init(socketId);
    }

    AbstractSocket::AbstractSocket(int socketId) {
        init(socketId);
    }


    void AbstractSocket::init(int socketId) {
        this->socketId = socketId;
    }
    
    unsigned int AbstractSocket::getSocketId() {
        return this->socketId;
    }

    void AbstractSocket::setReuseAddress(bool value) {
    
        if(value != reuseAddress) {
        
            // Set SO_REUSEADDR on a socket to true (1):
            // XXX: on Windows and sun this is a char instead of an int!
            #if defined(WIN32) || defined(sun)
                char optval = value ? '1': '0';
            #else
                int  optval = value ?  1 :  0 ;
            #endif

            if(setsockopt(getSocketId(), SOL_SOCKET, SO_REUSEADDR, &optval, 
                  sizeof(optval)) == -1)
            {
                DEBUG(std::cout << "AbstractSocket: setReuseAddress, " 
                        << "setsockopt failed - " << errno << std::endl;)
                throw Exception();
            }
            
            this->reuseAddress = value;
        }
    }
 
    std::string &AbstractSocket::getAddress() {
        return ipAddress;
    }
    
    int AbstractSocket::getPort() {
        return port;
    }
 
    
}


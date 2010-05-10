#ifndef _NLSOCKET_HPP_
#define _NLSOCKET_HPP_

#include "Shared.hpp"
#include "AbstractSocket.hpp"
#include "Packet.hpp"

#include <vector>
#include <string>
#include <iostream>

namespace NL {

    class Socket: public AbstractSocket {

        private:

        //friend class NLNetwork;
            
        // States:
        bool connected;

        // Buffers:
        std::vector<char> buffer_in; 
     
        bool packetExistsInBuffer();
        void recieveData();
        void sendData(const char *data, int length);
     
        public:

        // Constructor and destructor:
        Socket(std::string address, int port);
        Socket(int);
        virtual ~Socket();

        // Packet functions:
        Packet recievePacket();
        void   sendPacket(Packet &paket);
          
        // Connection functions:
        void connectTo(std::string addr, int port);

        // State functions:
        bool isConnected() { return connected; }
          
    };
    
}

#endif


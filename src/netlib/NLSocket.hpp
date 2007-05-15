#ifndef _NLSOCKET_H_
#define _NLSOCKET_H_

#include "netlib.h"

#include <vector>
#include <string>
#include <iostream>

class NLSocket
{
    private:

    friend class NLNetwork;
         
    // Id:
    unsigned int socketid;
 
    // States:
    bool listening;
    bool connected;
 
    bool gotsome;

    // Information:              
    std::string ip;
    int         port;

    // Buffers:
    std::vector<char> buffer_in; 
    std::vector<char> buffer_out;
 
    public:

    // Constructor and destructor:
    NLSocket();
    NLSocket(int);
    ~NLSocket();

    // Packet functions:        
    bool packet_exists();
    NLPacket packet_get(); 
    bool packet_put(NPacket);
        
    // Connection functions:
    bool connect_to(std::string addr, int port);
    bool listen_on(int port);

    // State functions:
    bool is_listening() { return listening; }
    bool is_connected() { return connected; }

    bool got_connection() { return listening && gotsome; }
 
    // Information:
    std::string get_ip();
    int get_port();
    int get_id() { return socketid; }

      
};

#endif

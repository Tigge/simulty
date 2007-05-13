#ifndef _NSOCKET_H_
#define _NSOCKET_H_

#include "netlib.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class NSocket
{
    private:

    friend class NNetwork;
         
    // Id:
    unsigned int socketid;
 
    // States:
    bool listening;
    bool connected;
 
    bool gotsome;

    // Information:              
    string ip;
    int    port;

    // Buffers:
    vector<char> buffer_in; 
    vector<char> buffer_out;
 
    public:

    // Constructor and destructor:
    NSocket();
    NSocket(int);
    ~NSocket();

    // Packet functions:        
    bool packet_exists();
    NPacket packet_get(); 
    bool packet_put(NPacket);
        
    // Connection functions:
    bool connect_to(string addr, int port);
    bool listen_on(int port);

    // State functions:
    bool is_listening() { return listening; }
    bool is_connected() { return connected; }

    bool got_connection() { return listening && gotsome; }
 
    // Information:
    string get_ip();
    int get_port();
    int get_id() { return socketid; }

      
};

#endif

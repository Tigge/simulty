#ifndef _PACKETRECEIVER_HPP_
#define _PACKETRECEIVER_HPP_

#include <queue>

#include "NL.hpp"

#include "SDL.h"
#include "SDL_thread.h"

#include "Runnable.hpp"

class PacketReceiver: public Runnable {
    
    private:

    SDL_mutex *mutex; 
    int        eventCode;
    
    std::queue<NL::Packet> packets;
    NL::Socket *socket;
    void       *data;

    public:

    PacketReceiver(NL::Socket *, int, void *);
    virtual ~PacketReceiver();

    NL::Packet popPacket();
    bool       havePacket();

    virtual void run();    
};

#endif


#ifndef _PACKETSENDER_HPP_
#define _PACKETSENDER_HPP_

#include <queue>

#include "NL.hpp"

#include "SDL.h"
#include "SDL_thread.h"

#include "Runnable.hpp"

class PacketSender: public Runnable {

    private:

    SDL_cond          *gotPacket;
    SDL_mutex         *mutex;
    
    std::queue<NL::Packet> packets;
    NL::Socket        *socket;
  
    public:

    PacketSender(NL::Socket *);
    virtual ~PacketSender();

    void pushPacket(NL::Packet);

    virtual void run();

};

#endif


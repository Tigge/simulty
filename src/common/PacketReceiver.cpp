#include "PacketReceiver.hpp"

PacketReceiver::PacketReceiver(NL::Socket *socket, int eventCode, void *data) {
    this->socket    = socket;
    this->data      = data;

    this->mutex     = SDL_CreateMutex();
    this->eventCode = eventCode;
}

PacketReceiver::~PacketReceiver() {

    SDL_LockMutex(mutex);    
    // TODO: delete all remaining packets
    SDL_UnlockMutex(mutex);

    SDL_DestroyMutex(this->mutex);
}

bool PacketReceiver::havePacket() {
    SDL_LockMutex(mutex);    
    bool havePacket = packets.size() >= 1;    
    SDL_UnlockMutex(mutex);
    return havePacket;
}

NL::Packet PacketReceiver::popPacket() {
    SDL_LockMutex(mutex);    
    NL::Packet packet = packets.front();    
    packets.pop();
    SDL_UnlockMutex(mutex);
    return packet;
}

void PacketReceiver::run() {

    while(true) {

        std::cout << "PacketReceiver::Waiting to receive packet" << std::endl;
        NL::Packet packet = socket->recievePacket();
        std::cout << "PacketReceiver::Got packet" << std::endl;

        SDL_LockMutex(mutex);
        packets.push(packet);
        std::cout << "PacketReceiver::There are now " << packets.size() << " packets" << std::endl;
        SDL_UnlockMutex(mutex);
        
        SDL_Event event;
        event.type = SDL_USEREVENT;
        event.user.code  = eventCode;
        event.user.data1 = data;
        event.user.data2 = this;
        SDL_PushEvent(&event);
        

    }
}


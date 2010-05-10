#include "PacketSender.hpp"

PacketSender::PacketSender(NL::Socket *socket) {
    this->socket    = socket;

    this->mutex     = SDL_CreateMutex();
    this->gotPacket = SDL_CreateCond();
}

PacketSender::~PacketSender() {
    SDL_DestroyCond(this->gotPacket);
    SDL_DestroyMutex(this->mutex);
}

    
void PacketSender::pushPacket(NL::Packet packet) {
    std::cout << "PacketSender: Pushing packet..." << std::endl;
    SDL_LockMutex(mutex);
    std::cout << "PacketSender: Before " << packets.size() << std::endl;
    packets.push(packet);
    std::cout << "PacketSender: After " << packets.size() << std::endl;
    SDL_UnlockMutex(mutex);
    SDL_CondSignal(gotPacket);
}
    

void PacketSender::run() {

    while(true) {
        
        SDL_LockMutex(mutex);
        if(packets.empty()) {
            SDL_CondWait(gotPacket, mutex);
        }
        
        std::cout << "PacketSender: Sending packet..." << std::endl;
        std::cout << "PacketSender: Getting one of the " << packets.size() << std::endl;
        std::cout.flush();
        
        NL::Packet packet = packets.front();
        packets.pop();
        std::cout << "PacketSender: Got one - now " << packets.size() << std::endl; 
        SDL_UnlockMutex(mutex);
        
        socket->sendPacket(packet);


    }
}

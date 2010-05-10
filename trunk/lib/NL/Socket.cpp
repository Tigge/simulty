#include "NL.hpp"

#include <cstring>

namespace NL {

    Socket::Socket(std::string address, int port) {

        connected = false;
        DEBUG(std::cout << "Socket: Constructor" << std::endl;)
         connectTo(address, port);
    }

    Socket::Socket(int socketId): AbstractSocket(socketId) {

        struct sockaddr_in address;
        socklen_t          addressLength = sizeof(struct sockaddr_in); 

        // Resolve ip and port
        getpeername(socketId, (struct sockaddr *)&address, &addressLength);  
  
        // Add ip and port info:
        this->ipAddress = inet_ntoa(address.sin_addr);
        this->port      = ntohs(address.sin_port);

        connected = true;

        DEBUG(std::cout << "Socket: Constructor (from id)" << std::endl;)
    }

    Socket::~Socket()
    {
        DEBUG(std::cout << "Socket: Destructor" << std::endl;)
    }
    
    void Socket::connectTo(std::string addr, int prt) {
    
        DEBUG(std::cout << "Socket: Connect to " << addr << ":" << prt << std::endl;)
        
        addr = Network::hostToAddress(addr);

        DEBUG(std::cout << "Address after lookup: " << addr << std::endl;)

        struct sockaddr_in theirstuff;   // will hold the destination addr

        theirstuff.sin_family = AF_INET;          // host byte order
        theirstuff.sin_port = htons(prt);   // short, network byte order
        theirstuff.sin_addr.s_addr = inet_addr(addr.c_str());
        memset(&theirstuff.sin_zero, 0, sizeof(theirstuff.sin_zero));

        // don't forget to error check the connect()
        if(connect(getSocketId(), (struct sockaddr *)&theirstuff, sizeof(struct sockaddr)) == -1)
        {
            DEBUG(std::cout << "Socket: Connect to, failed to do connect" << std::endl;)
            throw Exception();
        }

        DEBUG(std::cout << "Socket: Connect to " << addr << ":" << prt << ", done!" << std::endl;)

        connected = true;
     
        this->ipAddress = addr;
        this->port      = prt;
    }


    void Socket::recieveData() {

        char buf[255];
        int count = recv(getSocketId(), buf, 255, 0);
        
        if(count == 0) {
            DEBUG(std::cout << "NL::Socket.recieveData - A socket " 
                    << getSocketId() << " disconnected gracefully - " 
                    << errno << " - " << strerror(errno) << std::endl;)

            shutdown(getSocketId(), 2);
            throw Exception();
            //s->connected = false;     
        } else if(count == -1) {
            DEBUG(std::cout << "NL::Socket.recieveData - A socket " 
                    << getSocketId() << " errored out, disconnecting - " 
                    << errno << " - " << strerror(errno) << std::endl;)
            shutdown(getSocketId(), 2);
            throw Exception();
        } else {
            //cout << "Network: update. Socket has something to recieve" << endl;
            //buf[count] = '\0';
            //cout << "Network: Update. Got " << count << " chars on socket " << i << endl;
            //cout << "Network: Update. We got: '" << buf << "'" << endl;
            //cout << "Network: Update. Buffer before: '" << sockread->buffer << "'" << endl;
            //sockread->buffer += buf;
            for(int i = 0; i < count; i++)
            {
                buffer_in.push_back(buf[i]);
            }        
            //cout << " - we got " << count << " bytes" << endl;	                 
            //cout << "Network: update, buffert is now '" << (char *)&(sockread->buffer[0]) << "' (" << sockread->buffer.size() << ") bytes long" << endl;	
            //cout << "Network: Update. Buffer after: '" << sockread->buffer << "'" << endl;
        }    
    
    }

    Packet Socket::recievePacket() {

        DEBUG(std::cout << "NL::Socket.getPacket, getting enought data for a "
                << "packet" << std::endl;)
   
        // Get enough data
        while(!packetExistsInBuffer()) {
            recieveData();        
        }
        
        DEBUG(std::cout << "NL::Socket.getPacket, got one!" << std::endl;)

        unsigned char h[4]; for(int i = 0; i < 4; i++)h[i] = buffer_in[i];

        NLINT16 type   = (int)h[0]; h[0] = 0;
        NLINT32 length = ntohl( *((NLINT32 *)h) );

        Packet p; p.setType(type);

        DEBUG(std::cout << " - type is:   " << type << std::endl;)
        DEBUG(std::cout << " - length is: " << length << std::endl;)

        // Copy buffer from socket buffer to packet buffer:
        for(int i = 4; i < length + 4; i++)
        {
            p.buffer.push_back(buffer_in[i]);
        }

        
        DEBUG(std::cout << "Buffer contains: '";)
        DEBUG(for(unsigned int i = 0; i < buffer_in.size(); i++) {)
        DEBUG(    std::cout << buffer_in[i]; })
        DEBUG( std::cout << "' (" << buffer_in.size() << " chars long)" << std::endl;)
  
        // Erase buffert space occupied by the packet:
        buffer_in.erase(buffer_in.begin(), buffer_in.begin() + length + 4);
    

        
        DEBUG(std::cout << "Package contains: '";)
        DEBUG(for(unsigned int i = 0; i < p.buffer.size(); i++) {)
        DEBUG(    std::cout << p.buffer[i]; })
        DEBUG(std::cout << "' (" << p.buffer.size() << " chars long)" << std::endl;)
  
        return p;
    }

    bool Socket::packetExistsInBuffer()
    {
        if(buffer_in.size() > 4)
        {
            DEBUG(std::cout << "NL::Socket.packetExistsInBuffer, checking" 
                    << std::endl;)

            unsigned char h[4]; for(int i = 0; i < 4; i++)h[i] = buffer_in[i];

            //INT16 type   = (int)h[0]; 
            h[0] = 0;
            NLINT32 length = ntohl( *((NLINT32 *)h) );
            /*
           cout << "Socket: packet_exists, header is: ";
            for(int i = 0; i < 4; i++)
               cout << (int)h[i] << " ";
           cout << endl;
            
           cout << " - type is:   " << type << endl;
           cout << " - length is: " << length << endl;   
            */
            if(buffer_in.size() >= (unsigned int)length + 4)
            {
                DEBUG(std::cout << "NL::Socket: packetExistsInBuffer, got one!" 
                        << std::endl;)
                return true;
            }
        }
        return false;
    }



    void Socket::sendData(const char *data, int length) {
        
        int total = 0;
        do {
            int sent = send(getSocketId(), data + total, length - total, 0);
            if(sent == -1) {
                DEBUG(std::cout << "NL::Socket.sendData: Error while sending "
                    << "(" << total << " of " << length << ") - " << errno
                    << ":" << strerror(errno) << std::endl;)
                throw Exception();
            }
            total += sent;
            DEBUG(std::cout << "NL::Socket.sendData: Sent away " << sent 
                    << " bytes (" << total << " of " << length << ")" 
                    << std::endl;)
        
        } while(total != length);
    }



    void Socket::sendPacket(Packet &packet) {
        
        DEBUG(std::cout << "NL::Socket.sendPacket, sending package" << std::endl;)

        // Generate header:
        unsigned char header[4];

        *((NLINT32 *)header) = htonl(packet.getSize());
        header[0]            = (unsigned char)packet.getType();


        DEBUG(std::cout << "NL::Socket: packet_put, header is: ";)
        DEBUG(for(int i = 0; i < 4; i++) {)
        DEBUG(    std::cout << (int)header[i] << " "; })
        DEBUG(std::cout << std::endl;)

        // Send data and header:
        sendData((const char *)header, 4);
        sendData(&packet.buffer.front(), packet.buffer.size());

        DEBUG(std::cout << "NL::Socket: packet_put, successfullly -- "
            << packet.buffer.size() << " bytes" << std::endl;)

    }

}



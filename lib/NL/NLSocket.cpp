#include "NL.hpp"

NLSocket::NLSocket()
{
    //buffer = "";     
    connected = false;
    listening = false;
    gotsome   = false;
	
    ip = "";
    port = -1;
             
    DEBUG(std::cout << "NLSocket: Constructor" << std::endl;)
}

NLSocket::NLSocket(int socketid_new)
{
    //buffer = "";  
    socketid = socketid_new;

    listening = false;
    connected = true;
    gotsome   = false;

    DEBUG(std::cout << "NLSocket: Constructor (from id)" << std::endl;)
}

std::string NLSocket::get_ip()
{
	return ip;
}

int NLSocket::get_port()
{
	return port;
}

NLSocket::~NLSocket()
{
    DEBUG(std::cout << "NLSocket: Destructor" << std::endl;)
}
bool NLSocket::connect_to(std::string addr, int prt)
{
    DEBUG(std::cout << "NLSocket: Connect to " << addr << ":" << prt << std::endl;)
    
    addr = NLNetwork::host_to_address(addr);

    DEBUG(std::cout << "Address after lookup: " << addr << std::endl;)

    struct sockaddr_in theirstuff;   // will hold the destination addr

    theirstuff.sin_family = AF_INET;          // host byte order
    theirstuff.sin_port = htons(prt);   // short, network byte order
    theirstuff.sin_addr.s_addr = inet_addr(addr.c_str());
    theirstuff.sin_zero[0] = '\0'; theirstuff.sin_zero[1] = '\0'; theirstuff.sin_zero[2] = '\0'; theirstuff.sin_zero[3] = '\0'; theirstuff.sin_zero[4] = '\0'; theirstuff.sin_zero[5] = '\0'; theirstuff.sin_zero[6] = '\0'; theirstuff.sin_zero[7] = '\0';

    // don't forget to error check the connect()
    if(connect(socketid, (struct sockaddr *)&theirstuff, sizeof(struct sockaddr)) == -1)
    {
        DEBUG(std::cout << "NLSocket: Connect to, failed to do connect" << std::endl;)
        return false;
    }

    DEBUG(std::cout << "NLSocket: Connect to " << addr << ":" << prt << ", done!" << std::endl;)

    connected = true;
 
    ip = addr;
    port = prt;

    return true;
}


bool NLSocket::listen_on(int prt)
{
    DEBUG(std::cout << "NLSocket: Listen on, port " << prt << std::endl;)

    DEBUG(std::cout << "NLSocket: Listen, socket is " << socketid << std::endl;)

    struct sockaddr_in mystuff;

    mystuff.sin_family      = AF_INET;
    mystuff.sin_port        = htons(prt);
    mystuff.sin_addr.s_addr = INADDR_ANY;
    mystuff.sin_zero[0] = '\0'; mystuff.sin_zero[1] = '\0'; mystuff.sin_zero[2] = '\0'; mystuff.sin_zero[3] = '\0'; mystuff.sin_zero[4] = '\0'; mystuff.sin_zero[5] = '\0'; mystuff.sin_zero[6] = '\0'; mystuff.sin_zero[7] = '\0';

    if(bind(this->socketid, (struct sockaddr *)&mystuff, sizeof(struct sockaddr_in)) == -1)
    {
        DEBUG(std::cout << "NLSocket: Listen, bind on socket " << socketid << " failed - " << h_errno << " " << errno << std::endl;)
        return false;
    }

    DEBUG(std::cout << "NLSocket: Listen on, bind done" << std::endl;)

    if(listen(socketid, 10) == -1)
    {
        DEBUG(std::cout << "NLSocket: Listen, listen on socket " << socketid << " failed" << std::endl;)
        return false;
    }

    DEBUG(std::cout << "NLSocket: Listen on, listen done" << std::endl;)

    listening = true;
 
    ip = "127.0.0.1";
    port = prt;


 return true;
}

bool NLSocket::packet_exists()
{
    if(buffer_in.size() > 4)
    {
        DEBUG(std::cout << "NLSocket: packet_exists, checking" << std::endl;)

        unsigned char h[4]; for(int i = 0; i < 4; i++)h[i] = buffer_in[i];

        //INT16 type   = (int)h[0]; 
        h[0] = 0;
        NLINT32 length = ntohl( *((NLINT32 *)h) );
        /*
       cout << "NLSocket: packet_exists, header is: ";
        for(int i = 0; i < 4; i++)
           cout << (int)h[i] << " ";
       cout << endl;
        
       cout << " - type is:   " << type << endl;
       cout << " - length is: " << length << endl;   
        */
        if(buffer_in.size() >= (unsigned int)length + 4)
        {
            DEBUG(std::cout << "NLSocket: packet_exists, got one!" << std::endl;)
            return true;
        }
    }
    return false;
}

NLPacket NLSocket::packet_get()
{
    DEBUG(std::cout << "NLSocket: packet_get, checking" << std::endl;)

    if(packet_exists())
    {
        DEBUG(std::cout << "NLSocket: packet_get, got one!" << std::endl;)

        unsigned char h[4]; for(int i = 0; i < 4; i++)h[i] = buffer_in[i];

        NLINT16 type   = (int)h[0]; h[0] = 0;
        NLINT32 length = ntohl( *((NLINT32 *)h) );

        NLPacket p; p.setType(type);

        DEBUG(std::cout << " - type is:   " << type << std::endl;)
        DEBUG(std::cout << " - length is: " << length << std::endl;)

        // Copy buffer from socket buffer to packet buffer:
        for(int i = 4; i < length + 4; i++)
        {
            p.buffer.push_back(buffer_in[i]);
        }

        
        DEBUG(std::cout << "Buffer contains: '";)
        DEBUG(for(unsigned int i = 0; i < buffer_in.size(); i++))
        DEBUG(    std::cout << buffer_in[i];)
        DEBUG( std::cout << "' (" << buffer_in.size() << " chars long)" << std::endl;)
  
        // Erase buffert space occupied by the packet:
        buffer_in.erase(buffer_in.begin(), buffer_in.begin() + length + 4);
	  

        
        DEBUG(std::cout << "Package contains: '";)
        DEBUG(for(int i = 0; i < p.buffer.size(); i++))
        DEBUG(    std::cout << p.buffer[i];)
        DEBUG(std::cout << "' (" << p.buffer.size() << " chars long)" << std::endl;)
  
        return p;
   
    }

    DEBUG(std::cout << "NLSocket: packet_get, no packet exists..." << std::endl;)

    NLPacket p;
    return p;
}


bool NLSocket::packet_put(NLPacket p)
{


    DEBUG(std::cout << "NLSocket: packet_put, putting package" << std::endl;)
    if(connected)
    {
        // Generate header:

        unsigned char h[4];

        *((NLINT32 *)h) = htonl(p.getSize());
        h[0]            = (unsigned char)p.getType();


        DEBUG(std::cout << "NLSocket: packet_put, header is: ";)
        DEBUG(for(int i = 0; i < 4; i++))
        DEBUG(    std::cout << (int)h[i] << " ";)
        DEBUG(std::cout << std::endl;)

        // Put header in outgoing buffer:
        buffer_out.insert(buffer_out.end(), h, h + 4);

        // Put contents in outgoing buffer:
        buffer_out.insert(buffer_out.end(), p.buffer.begin(), p.buffer.end());

        DEBUG(std::cout << "NLSocket: packet_put, successfull, header: " << h << ", new buffer is " << buffer_out.size() << " long" << std::endl;)

        return true;
    }
    else
    {
        DEBUG(std::cout << "NLSocket: packet_put, socket was not connected" << std::endl;)
        return false;
    }
}





#include "netlib.h"

NSocket::NSocket()
{
    //buffer = "";     
    connected = false;
    listening = false;
    gotsome   = false;
	
    ip = "";
    port = -1;
             
    DEBUG(cout << "NSocket: Constructor" << endl;)
}

NSocket::NSocket(int socketid_new)
{
    //buffer = "";  
    socketid = socketid_new;

    listening = false;
    connected = true;
    gotsome   = false;

    DEBUG(cout << "NSocket: Constructor (from id)" << endl;)
}

std::string NSocket::get_ip()
{
	return ip;
}

int NSocket::get_port()
{
	return port;
}

NSocket::~NSocket()
{
    DEBUG(cout << "NSocket: Destructor" << endl;)
}
bool NSocket::connect_to(std::string addr, int prt)
{
    DEBUG(cout << "NSocket: Connect to " << addr << ":" << prt << endl;)
    
    addr = NNetwork::host_to_address(addr);

    DEBUG(cout << "Address after lookup: " << addr << endl;)

    struct sockaddr_in theirstuff;   // will hold the destination addr

    theirstuff.sin_family = AF_INET;          // host byte order
    theirstuff.sin_port = htons(prt);   // short, network byte order
    theirstuff.sin_addr.s_addr = inet_addr(addr.c_str());
    theirstuff.sin_zero[0] = '\0'; theirstuff.sin_zero[1] = '\0'; theirstuff.sin_zero[2] = '\0'; theirstuff.sin_zero[3] = '\0'; theirstuff.sin_zero[4] = '\0'; theirstuff.sin_zero[5] = '\0'; theirstuff.sin_zero[6] = '\0'; theirstuff.sin_zero[7] = '\0';

    // don't forget to error check the connect()
    if(connect(socketid, (struct sockaddr *)&theirstuff, sizeof(struct sockaddr)) == -1)
    {
        DEBUG(cout << "NSocket: Connect to, failed to do connect" << endl;)
        return false;
    }

    DEBUG(cout << "NSocket: Connect to " << addr << ":" << prt << ", done!" << endl;)

    connected = true;
 
    ip = addr;
    port = prt;

    return true;
}


bool NSocket::listen_on(int prt)
{
    DEBUG(cout << "NSocket: Listen on, port " << prt << endl;)

    DEBUG(cout << "NSocket: Listen, socket is " << socketid << endl;)

    struct sockaddr_in mystuff;

    mystuff.sin_family      = AF_INET;
    mystuff.sin_port        = htons(prt);
    mystuff.sin_addr.s_addr = INADDR_ANY;
    mystuff.sin_zero[0] = '\0'; mystuff.sin_zero[1] = '\0'; mystuff.sin_zero[2] = '\0'; mystuff.sin_zero[3] = '\0'; mystuff.sin_zero[4] = '\0'; mystuff.sin_zero[5] = '\0'; mystuff.sin_zero[6] = '\0'; mystuff.sin_zero[7] = '\0';

    if(bind(this->socketid, (struct sockaddr *)&mystuff, sizeof(struct sockaddr_in)) == -1)
    {
        DEBUG(cout << "NSocket: Listen, bind on socket " << socketid << " failed - " << h_errno << " " << errno << endl;)
        return false;
    }

    DEBUG(cout << "NSocket: Listen on, bind done" << endl;)

    if(listen(socketid, 10) == -1)
    {
        DEBUG(cout << "NSocket: Listen, listen on socket " << socketid << " failed" << endl;)
        return false;
    }

    DEBUG(cout << "NSocket: Listen on, listen done" << endl;)

    listening = true;
 
    ip = "127.0.0.1";
    port = prt;


 return true;
}

bool NSocket::packet_exists()
{
    if(buffer_in.size() > 4)
    {
        DEBUG(cout << "NSocket: packet_exists, checking" << endl;)

        unsigned char h[4]; for(int i = 0; i < 4; i++)h[i] = buffer_in[i];

        //INT16 type   = (int)h[0];
        INT32 length = ntohl( *((INT32 *)h) & 0xFF000000 );
        /*
       cout << "NSocket: packet_exists, header is: ";
        for(int i = 0; i < 4; i++)
           cout << (int)h[i] << " ";
       cout << endl;
        
       cout << " - type is:   " << type << endl;
       cout << " - length is: " << length << endl;   
        */
        if(buffer_in.size() >= (unsigned int)length + 4)
        {
            DEBUG(cout << "NSocket: packet_exists, got one!" << endl;)
            return true;
        }
    }
    return false;
}

NPacket NSocket::packet_get()
{
    DEBUG(cout << "NSocket: packet_get, checking" << endl;)

    if(packet_exists())
    {
        DEBUG(cout << "NSocket: packet_get, got one!" << endl;)

        unsigned char h[4]; for(int i = 0; i < 4; i++)h[i] = buffer_in[i];

        INT16 type   = (int)h[0];
        INT32 length = ntohl( *((INT32 *)h) & 0xFF000000 );

        NPacket p; p.setType(type);

        DEBUG(cout << " - type is:   " << type << endl;)
        DEBUG(cout << " - length is: " << length << endl;)

        // Copy buffer from socket buffer to packet buffer:
        for(int i = 4; i < length + 4; i++)
        {
            p.buffer.push_back(buffer_in[i]);
        }

        
        DEBUG(cout << "Buffer contains: '";)
        DEBUG(for(int i = 0; i < buffer_in.size(); i++))
        DEBUG(    cout << buffer_in[i];)
        DEBUG( cout << "' (" << buffer_in.size() << " chars long)" << endl;)
  
        // Erase buffert space occupied by the packet:
        buffer_in.erase(buffer_in.begin(), buffer_in.begin() + length + 4);
	  

        
        DEBUG(cout << "Package contains: '";)
        DEBUG(for(int i = 0; i < p.buffer.size(); i++))
        DEBUG(    cout << p.buffer[i];)
        DEBUG(cout << "' (" << p.buffer.size() << " chars long)" << endl;)
  
        return p;
   
    }

    DEBUG(cout << "NSocket: packet_get, no packet exists..." << endl;)

    NPacket p;
    return p;
}


bool NSocket::packet_put(NPacket p)
{


    DEBUG(cout << "NSocket: packet_put, putting package" << endl;)
    if(connected)
    {
        // Generate header:

        unsigned char h[4];

        *((INT32 *)h) = htonl(p.getSize());
        h[0]          = (unsigned char)p.getType();


        DEBUG(cout << "NSocket: packet_put, header is: ";)
        DEBUG(for(int i = 0; i < 4; i++))
        DEBUG(    cout << (int)h[i] << " ";)
        DEBUG(cout << endl;)

        // Put header in outgoing buffer:
        buffer_out.insert(buffer_out.end(), h, h + 4);

        // Put contents in outgoing buffer:
        buffer_out.insert(buffer_out.end(), p.buffer.begin(), p.buffer.end());

        DEBUG(cout << "NSocket: packet_put, successfull, header: " << h << ", new buffer is " << buffer_out.size() << " long" << endl;)

        return true;
    }
    else
    {
        DEBUG(cout << "NSocket: packet_put, socket was not connected" << endl;)
        return false;
    }
}





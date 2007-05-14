
#include "netlib.h"

NPacket::NPacket()
{
    type = NPACKET_TYPE_NONE;
    buffer.reserve(10);
}

NPacket::NPacket(int t)
{
    setType(t);
}

NPacket::~NPacket()
{
    
}

int NPacket::getType()
{
    return type;
}

bool NPacket::setType(int val)
{
    type = val;
    return true; // (TODO), better checking
}

int NPacket::getSize()
{
    return buffer.size();
}

// 32 bit numbers (4 chars)
NPacket &NPacket::operator<< (const NL_INT32 &val)
{ 
    unsigned char b[4]; *((NL_INT32 *)b) = htonl(val);
    for(int i = 0; i < 4; i++)buffer.push_back((char)b[i]);

    return *this; 
}

NPacket &NPacket::operator>> (NL_INT32 &val)
{
    unsigned char b[4]; for(int i = 0; i < 4; i++)b[i] = buffer[i];
    val = ntohl( *((NL_INT32 *)b) );

    buffer.erase(buffer.begin(), buffer.begin() + 4);        
    
    return *this;
}

// 16 bit numbers (2 chars)
NPacket &NPacket::operator<< (const NL_INT16 &val)
{ 
    unsigned char b[2]; *((NL_INT16 *)b) = htons(val);
    for(int i = 0; i < 2; i++)buffer.push_back(b[i]);

    return *this; 
}

NPacket &NPacket::operator>> (NL_INT16 &val)
{
    unsigned char b[2]; for(int i = 0; i < 2; i++)b[i] = buffer[i];
    val = ntohs( *((NL_INT16 *)b) );

    buffer.erase(buffer.begin(), buffer.begin() + 2);
    
    return *this;
}


NPacket &NPacket::operator<< (const char &val) 
{ 
    buffer.push_back(val); 
    return *this; 
}
NPacket &NPacket::operator<< (const unsigned char &val) 
{ 
    buffer.push_back(val); 
    return *this; 
}


NPacket &NPacket::operator>> (char &val) 
{
    //std::cout << "output char (" << buffer.size() << ")" << std::endl;
    val = buffer[0]; buffer.erase(buffer.begin()); 
    return *this; 
}

NPacket &NPacket::operator>> (unsigned char &val)
{
    //std::cout << "output unsigned char (" << buffer.size() << ")" << std::endl;
    val = buffer[0]; buffer.erase(buffer.begin()); 
    return *this; 
}


NPacket &NPacket::operator<< (const std::string &val) 
{
    for(unsigned int i = 0; i < val.length(); i++)
        buffer.push_back(val[i]); 
    buffer.push_back('\0');
    return *this; 
}

NPacket &NPacket::operator>> (std::string &val)
{
    //std::cerr << "getting string" << std::endl;
    unsigned int pos = 0;
    for(;pos < buffer.size(); pos++)
    {
        if(buffer[pos] == '\0')break;
    }

    //std::cerr << "..." << std::endl;    
    //pos = buffer.size(); // TODO remove;

    for(unsigned int i = 0; i < pos; i++)
    {
        val.push_back(buffer[i]);
    }

    //std::cerr << "..." << std::endl;

    buffer.erase(buffer.begin(), buffer.begin() + pos);

    //std::cerr << "..." << std::endl;

    return *this;
}

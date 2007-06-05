
#include "NL.hpp"

NLPacket::NLPacket() {
  type = NLPACKET_TYPE_NONE;
  buffer.reserve(10);
}

NLPacket::NLPacket(int t) {
setType(t);
}

NLPacket::~NLPacket() {
    
}

int NLPacket::getType() {
return type;
}

bool NLPacket::setType(int val)
{
    type = val;
    return true; // (TODO), better checking
}

int NLPacket::getSize()
{
    return buffer.size();
}

// 32 bit numbers (4 chars)
NLPacket &NLPacket::operator<< (const NLINT32 &val) { 
    unsigned char b[4]; *((NLINT32 *)b) = htonl(val);
    for(int i = 0; i < 4; i++)buffer.push_back((char)b[i]);

    return *this; 
}

NLINT32 NLPacket::nextInt32() {
  if(buffer.size() >= 4) {
    NLINT32 val = ntohl(*((NLINT32 *)&buffer[0]));
    buffer.erase(buffer.begin(), buffer.begin() + 4);
    return val;
  } 
  throw NLException();
}


NLPacket &NLPacket::operator>> (NLINT32 &val) {

    unsigned char b[4]; for(int i = 0; i < 4; i++)b[i] = buffer[i];
    val = ntohl( *((NLINT32 *)b) );

    buffer.erase(buffer.begin(), buffer.begin() + 4);        
    
    return *this;
}

// 16 bit numbers (2 chars)
NLPacket &NLPacket::operator<< (const NLINT16 &val)
{ 
    unsigned char b[2]; *((NLINT16 *)b) = htons(val);
    for(int i = 0; i < 2; i++)buffer.push_back(b[i]);

    return *this; 
}

NLPacket &NLPacket::operator>> (NLINT16 &val)
{
    unsigned char b[2]; for(int i = 0; i < 2; i++)b[i] = buffer[i];
    val = ntohs( *((NLINT16 *)b) );

    buffer.erase(buffer.begin(), buffer.begin() + 2);
    
    return *this;
}

NLINT16 NLPacket::nextInt16() {
  if(buffer.size() >= 2) {
    NLINT16 val = ntohs(*((NLINT16 *)&buffer[0]));
    buffer.erase(buffer.begin(), buffer.begin() + 2);
    return val;
  } 
  throw NLException();
}


NLPacket &NLPacket::operator<< (const char &val) 
{ 
    buffer.push_back(val); 
    return *this; 
}
NLPacket &NLPacket::operator<< (const unsigned char &val) 
{ 
    buffer.push_back(val); 
    return *this; 
}


NLPacket &NLPacket::operator>> (char &val) 
{
    //std::cout << "output char (" << buffer.size() << ")" << std::endl;
    val = buffer[0]; buffer.erase(buffer.begin()); 
    return *this; 
}

NLPacket &NLPacket::operator>> (unsigned char &val)
{
    //std::cout << "output unsigned char (" << buffer.size() << ")" << std::endl;
    val = buffer[0]; buffer.erase(buffer.begin()); 
    return *this; 
}

char NLPacket::nextChar() {
  if(buffer.size() >= 1) {
    char val = buffer[0];
    buffer.erase(buffer.begin(), buffer.begin() + 1);
    return val;
  } 
  throw NLException();
}


NLPacket &NLPacket::operator<< (const std::string &val) 
{
    for(unsigned int i = 0; i < val.length(); i++)
        buffer.push_back(val[i]); 
    buffer.push_back('\0');
    return *this; 
}

NLPacket &NLPacket::operator>> (std::string &val)
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

std::string NLPacket::nextString() {

  // Find end of string:
  unsigned int pos;
  for(pos = 0;pos < buffer.size(); pos++) {
    if(buffer[pos] == '\0')break;
  }
  
  // No end of string.. weird
  if(pos == buffer.size())throw NLException();

  // Fill and return string:
  std::string val;
  for(unsigned int i = 0; i < pos; i++) {
    val.push_back(buffer[i]);
  }  
  return val;
}


#ifndef _NLPACKET_H_
#define _NLPACKET_H_

#include "NL.hpp"

#include <iostream>
#include <vector>
#include <string>

#define NLINT16 short
#define NLINT32 int

#define NLPACKET_TYPE_NONE 0

class NLPacket { 

    friend class NLSocket;

    std::vector<char> buffer;
    int type;

    public:

    NLPacket();
    NLPacket(int t);

    virtual ~NLPacket();

    int  getType();
    bool setType(int);
    int  getSize();

    // 32 bit numbers (4 chars)
    NLPacket &operator<< (const NLINT32 &val);
    NLPacket &operator>> (NLINT32 &val);


    // 16 bit numbers (2 chars)
    NLPacket &operator<< (const NLINT16 &val);
    NLPacket &operator>> (NLINT16 &val);
   
    // Single chars (1 char)
    NLPacket &operator<< (const char &val);
    NLPacket &operator<< (const unsigned char &val);

    NLPacket &operator>> (char &val);
    NLPacket &operator>> (unsigned char &val);


    // Strings (n chars, end with \0)
    NLPacket &operator<< (const std::string &val);
    NLPacket &operator>> (std::string &val);

    // DEBUG, todo remove
    void print()
    {
        std::cout << "-Content of packet: ";
        for(unsigned int i = 0; i < buffer.size(); i++) std::cout << buffer[i];
        std::cout << std::endl;
        for(unsigned int i = 0; i < buffer.size(); i++) std::cout << (int)buffer[i] << " ";
        std::cout << std::endl;
    }


};

#endif

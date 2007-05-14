
#ifndef _PACKET_H_
#define _PACKET_H_

#include "netlib.h"

#include <iostream>
#include <vector>
#include <string>

#define NL_INT16 short
#define NL_INT32 int

#define NPACKET_TYPE_NONE 0

class NPacket { 

    friend class NSocket;

    std::vector<char> buffer;
    int type;

    public:

    NPacket();
    NPacket(int t);

    virtual ~NPacket();

    int  getType();
    bool setType(int);
    int  getSize();

    // 32 bit numbers (4 chars)
    NPacket &operator<< (const NL_INT32 &val);
    NPacket &operator>> (NL_INT32 &val);


    // 16 bit numbers (2 chars)
    NPacket &operator<< (const NL_INT16 &val);
    NPacket &operator>> (NL_INT16 &val);
   
    // Single chars (1 char)
    NPacket &operator<< (const char &val);
    NPacket &operator<< (const unsigned char &val);

    NPacket &operator>> (char &val);
    NPacket &operator>> (unsigned char &val);


    // Strings (n chars, end with \0)
    NPacket &operator<< (const std::string &val);
    NPacket &operator>> (std::string &val);

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

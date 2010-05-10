
#ifndef _Packet_HPP_
#define _Packet_HPP_

#include "Shared.hpp"
#include "Exception.hpp"

#include <iostream>
#include <vector>
#include <string>

#define NLINT16 short
#define NLINT32 int

#define Packet_TYPE_NONE 0

namespace NL {

    class Packet { 

        friend class Socket;

        std::vector<char> buffer;
        int type;

        public:

        Packet();
        Packet(int t);

        virtual ~Packet();

        int  getType();
        bool setType(int);
        int  getSize();

        // 32 bit numbers (4 chars)
        Packet &operator<< (const NLINT32 &val);
        Packet &operator>> (NLINT32 &val);
        NLINT32 nextInt32();

        // 16 bit numbers (2 chars)
        Packet &operator<< (const NLINT16 &val);
        Packet &operator>> (NLINT16 &val);
        NLINT16 nextInt16();
       
        // Single chars (1 char)
        Packet &operator<< (const char &val);
        Packet &operator<< (const unsigned char &val);

        Packet &operator>> (char &val);
        Packet &operator>> (unsigned char &val);
        char nextChar();

        // Strings (n chars, end with \0)
        Packet &operator<< (const std::string &val);
        Packet &operator>> (std::string &val);
        std::string nextString();

        // DEBUG, todo remove
        void print() {
            std::cout << "-Content of packet: ";
            for(unsigned int i = 0; i < buffer.size(); i++) std::cout << buffer[i];
            std::cout << std::endl;
            for(unsigned int i = 0; i < buffer.size(); i++) std::cout << (int)buffer[i] << " ";
            std::cout << std::endl;
        }
        
        std::string getData() {
          std::string s;
          for(unsigned int i = 0; i < buffer.size(); i++)
            s.push_back(buffer[i]);
          return s;
        }
        
        const char *getBytes() {
            return &buffer.front();
        }


    };
}

#endif

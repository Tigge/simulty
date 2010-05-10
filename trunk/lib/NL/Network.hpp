#ifndef _NLNETWORK_H_
#define _NLNETWORK_H_

#include "Socket.hpp"
#include "NL.hpp"

#include <vector>
#include <iostream>
#include <cstdio>


namespace NL {

    class Network {

        private:

        static bool init;         
            
        public:
              
        Network();
        virtual ~Network();  

        static std::string hostToAddress(std::string host);
        static std::string addressToHost(std::string address); 


    };
}

#endif


#ifndef _NLABSTRACTSOCKET_HPP_
#define _NLABSTRACTSOCKET_HPP_

//#include "NL.hpp"

#include "Shared.hpp"
#include "Exception.hpp"
#include <string>

namespace NL {

    class AbstractSocket {

        private:
        
        static const bool DEFAULT_SO_REUSEADDR = true;
        static const int  DEFAULT_SO_TIMEOUT   = 10;
        static const int  DEFAULT_SO_RCVBUF    = 10;

        unsigned int socketId;
        bool         reuseAddress;
        int          timeout;
        int          receiveBffer;

        void init(int socketId);

        protected:
                
        std::string  ipAddress;
        int          port;
        
        public:
        
        AbstractSocket();
        AbstractSocket(int socketId);

        /** 
         *
         * @return  The unique socket id
         */
        unsigned int getSocketId();
        std::string &getAddress();
        int          getPort();

        void         setReuseAddress(bool);

    };
}

#endif


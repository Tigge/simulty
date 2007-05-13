#include "socket_m.h"
#include <vector>
#include <iostream>

#ifndef _NNETWORK_H_
#define _NNETWORK_H_

#ifdef WIN32
 #include "winsock.h"
#else
 #include <netdb.h>
 #include <errno.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
#endif


class NNetworkMedium {

    

    public:

    NNetworkMedium();
    virtual ~NNetworkMedium();

    virtual NSocketMedium *add();
    virtual NSocketMedium *add(NSocketMedium *);

    virtual bool update();

}

#ifndef _NNETWORK_H_
#define _NNETWORK_H_

#include "socket.h"
#include "netlib.h"

#include <vector>
#include <iostream>

#ifdef WIN32
 #include "winsock.h"
 #define  socklen_t int 
#else
 #include <netdb.h>
 #include <errno.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
#endif

class NNetwork
{
 private:
 
 static bool init;         
 
 fd_set       filedescset;
 unsigned int filedescmax;
 
 std::vector<NSocket *> sockets;

 bool socket_init(NSocket *);
 bool socket_fixreuse(NSocket *);
 bool socket_addlist(NSocket *);
      
 public:
        
 NNetwork();
 virtual ~NNetwork();  

 NSocket *get_by_id(unsigned int n);
 NSocket *get_by_n(unsigned int n);
 
 int count();
 
 virtual bool update(int uwait); 

 virtual NSocket *add();
 virtual NSocket *add(NSocket *);
 
 virtual bool del_by_n(unsigned int n);
 virtual bool del_by_id(unsigned int n);
 
 // Move these two! (or perhaps make them static, hmm) TODO
 static std::string host_to_address(std::string host);
 static std::string address_to_host(std::string address); 

 virtual NSocket *operator[](int);

};

#endif

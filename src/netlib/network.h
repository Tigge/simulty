#ifndef _NNETWORK_H_
#define _NNETWORK_H_

#include "socket.h"
#include "netlib.h"

#include <vector>
#include <iostream>

using namespace std;

class NNetwork
{
 private:
 
 static bool init;         
 
 fd_set       filedescset;
 unsigned int filedescmax;
 
 vector<NSocket *> sockets;

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
 static string host_to_address(string host);
 static string address_to_host(string address); 

 virtual NSocket *operator[](int);

};

#endif

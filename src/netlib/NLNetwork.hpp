#ifndef _NLNETWORK_H_
#define _NLNETWORK_H_

#include "NLSocket.hpp"
#include "NL.hpp"

#include <vector>
#include <iostream>

class NLNetwork {

 private:
 
 static bool init;         
 
 fd_set       filedescset;
 unsigned int filedescmax;
 
 std::vector<NLSocket *> sockets;

 bool socket_init(NLSocket *);
 bool socket_fixreuse(NLSocket *);
 bool socket_addlist(NLSocket *);
      
 public:
        
 NLNetwork();
 virtual ~NLNetwork();  

 NLSocket *get_by_id(unsigned int n);
 NLSocket *get_by_n(unsigned int n);
 
 int count();
 
 virtual bool update(int uwait); 

 virtual NLSocket *add();
 virtual NLSocket *add(NLSocket *);
 
 virtual bool del_by_n(unsigned int n);
 virtual bool del_by_id(unsigned int n);
 
 // Move these two! (or perhaps make them static, hmm) TODO
 static std::string host_to_address(std::string host);
 static std::string address_to_host(std::string address); 

 virtual NLSocket *operator[](int);

};

#endif

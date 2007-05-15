#ifndef _NL_H_
#define _NL_H_

#ifdef DEBUGMODE
  #define DEBUG(X) X    
#else   
  #define DEBUG(X)   
#endif

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

#include "NLExeption.hpp"
#include "NLPacket.hpp"
#include "NLSocket.hpp"
#include "NLNetwork.hpp"

#endif

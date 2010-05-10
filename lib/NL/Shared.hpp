#ifdef DEBUGMODE
  #define DEBUG(X) X std::cout.flush();
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


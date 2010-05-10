#include "Network.hpp"

namespace NL {
    bool Network::init = false;
    Network::Network()
    {
     //cout << "Network: Constructor" << endl;
     if(!init)
     {
     //cout << "Network: Init first time" << endl;

      #ifdef WIN32  
       WSADATA data;
       WSAStartup(MAKEWORD(2, 2), &data);  
      #endif          
      
      init = true;
     }                 
     
     //FD_ZERO(&filedescset);
     //filedescmax = 0;
     
    }

    Network::~Network()
    {
     //cout << "Network: Destructor" << endl;

     #ifdef WIN32  
      WSADATA data;
      WSAStartup(MAKEWORD(2, 2), &data);  
     #endif                         
    }

    std::string Network::hostToAddress(std::string host)
    {
     struct hostent *h;  
     if((h = gethostbyname(host.c_str())) == NULL)
     {
      //cout << "HNF: " << HOST_NOT_FOUND << endl;
      //cout << "ND: " << NO_DATA << endl;
      //cout << "NR: " << NO_RECOVERY << endl;    
      //cout << "TA: " << TRY_AGAIN << endl;  
      //cout << "Errno: " << h_errno << endl;
      return NULL; 
     }
     
     std::string address; 
     address = inet_ntoa(*((struct in_addr *)h->h_addr));
           
     return address;       
    }

    std::string Network::addressToHost(std::string address)
    {
     struct in_addr addr;
     addr.s_addr = inet_addr(address.c_str());
     
     struct hostent *h;      
     if((h = gethostbyaddr((char *)&addr.s_addr, sizeof(addr.s_addr), AF_INET)) == NULL)
     {
      return NULL;
     } 
     return (std::string)h->h_name;
    }

}


#include "network.h"


bool NNetwork::init = false;
NNetwork::NNetwork()
{
 //cout << "NNetwork: Constructor" << endl;
 if(!init)
 {
 //cout << "NNetwork: Init first time" << endl;

  #ifdef WIN32  
   WSADATA data;
   WSAStartup(MAKEWORD(2, 2), &data);  
  #endif          
  
  init = true;
 }                 
 
 FD_ZERO(&filedescset);
 filedescmax = 0;
 
}

NNetwork::~NNetwork()
{
 //cout << "NNetwork: Destructor" << endl;

 #ifdef WIN32  
  WSADATA data;
  WSAStartup(MAKEWORD(2, 2), &data);  
 #endif                         


 for(int i = sockets.size() - 1; i >= 0; i--)
 {
  //cout << "NNetwork: Deleting socket " << i << endl;
  delete sockets[i];
  sockets.pop_back();

 } 
}


bool NNetwork::socket_init(NSocket *s)
{
    int res = socket(PF_INET, SOCK_STREAM, 0);
    if(res  == -1)
    {
        //cout << "NNetwork: socket_init, failed to init socket - " << h_errno << endl;
        return false;
    }
    s->socketid = res;
    return true;
}

bool NNetwork::socket_fixreuse(NSocket *s)
{
    // set SO_REUSEADDR on a socket to true (1):
    // Note: on Windows and sun this is a char instead of an int!
    #if defined(WIN32) || defined(sun)
        char optval = '1';
    #else
        int optval = 1;
    #endif

    if(setsockopt(s->socketid, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
        //cout << "NNetwork: socket_fixreuse, setsockopt failed - " << errno << endl;
        return false;
    }
    return true;
}

bool NNetwork::socket_addlist(NSocket *s)
{
    //cout << "NNetwork: socket_add, successful (got id " << s->socketid << ")" << endl;
    sockets.push_back(s);

    // Adjust the socket description set (max and set):
    if(s->socketid > filedescmax)filedescmax = s->socketid;
    FD_SET(s->socketid, &filedescset);

    return true;
}


bool NNetwork::del_by_n(unsigned int n)
{
 if(n < sockets.size() && n >= 0)
 {
  sockets.erase(sockets.begin() + n); 	
  return true;	 
 }
 else
 {
  return false;
 } 
}

bool NNetwork::del_by_id(unsigned int n)
{
 for(unsigned int i = 0; i < sockets.size(); i++)
 {
  if(sockets[i]->socketid == n)
  {
   return del_by_n(i);
  }
 }	
 return false;
}

int NNetwork::count()
{
 ////cout << "NNetwork: Socket count is " << sockets.size() << endl;
 return sockets.size();
}

bool NNetwork::update(int uwait)
{
    //cout << "NNetwork: Update" << endl;
    fd_set read  = filedescset;
    fd_set write = filedescset;

    //cout << "NNetwork: File desc max is " << filedescmax << endl;

    struct timeval tv;
    tv.tv_sec = uwait / 1000;
    tv.tv_usec = uwait % 1000;

    int count = 0;
    if((count = select(filedescmax + 1, &read, &write, NULL, &tv)) == -1)
    {
        //cout << "NNetwork: Update, select failed " << errno << endl;
        return false;
    }

    //cout << "NNetwork: Update, # " << count << " sockets had an update" << endl;

    for(unsigned int i = 0, c = 0; i <= filedescmax && c < (unsigned int)count; i++)
    {
        if(FD_ISSET(i, &read) || FD_ISSET(i, &write))
        {
            NSocket *s = get_by_id(i);
            //cout << "NNetwork: Update. An update has hapend on socket with id" << i << endl;            
            
            c++;
            if(FD_ISSET(i, &read))
            {
              
                if(s->listening)
                {
                    //cout << "NNetwork: Update. We got an incomming connection on socket with id" << i << endl;
                    s->gotsome = true;
                }
                else if(s->connected)
                {
                    char buf[255];
                    int count = recv(s->socketid, buf, 255, 0);

                    if(count == 0)
                    {
                        //cout << "NNetwork: Update. A socket " << i << " disconnected gracefully" << endl;
	    
                        shutdown(s->socketid, 2);
                        s->connected = false;     
                        FD_CLR(s->socketid, &filedescset);
                    }
                    else if(count == -1)
                    {
                        //cout << "NNetwork: Update. A socket " << i << " errored out, disconnecting" << endl;
                        shutdown(s->socketid, 2);
                        s->connected = false; 
                        FD_CLR(s->socketid, &filedescset);
                    }
                    else
                    {
                        //cout << "NNetwork: update. Socket has something to recieve" << endl;
                        //buf[count] = '\0';
                        //cout << "NNetwork: Update. Got " << count << " chars on socket " << i << endl;
                        //cout << "NNetwork: Update. We got: '" << buf << "'" << endl;
                        //cout << "NNetwork: Update. Buffer before: '" << sockread->buffer << "'" << endl;
                        //sockread->buffer += buf;
                        for(int i = 0; i < count; i++)
                        {
                            s->buffer_in.push_back(buf[i]);
                        }
                        //cout << " - we got " << count << " bytes" << endl;	                 
                        //cout << "NNetwork: update, buffert is now '" << (char *)&(sockread->buffer[0]) << "' (" << sockread->buffer.size() << ") bytes long" << endl;	
                        //cout << "NNetwork: Update. Buffer after: '" << sockread->buffer << "'" << endl;
                    }
                }
            }
            else if(FD_ISSET(i, &write))
            {
                if(s->connected && !s->buffer_out.empty())
                {   
                    //cout << "NNetwork: update. Socket has something to send" << endl;
                    int sent;
                    if((sent = send(s->socketid, (const char *)&(s->buffer_out[0]), s->buffer_out.size(), 0)) == -1)
                    {
                        //cout << " - error while sending" << endl;
                    }
                    else
                    {
                        //cout << " - sent away " << sent << " bytes" << endl;
                        s->buffer_out.erase(s->buffer_out.begin(), s->buffer_out.begin() + sent);
                    }
                    // Send data
                }
            }
        }
    }
    return true;
}
                  
NSocket *NNetwork::get_by_n(unsigned int n)
{
    if(n >= 0 && n < sockets.size())
    {
        //cout << "NNetwork: get_by_n, found with " << n << endl;
        return sockets[n];
    }
    else
    {
        return NULL;
    }
}

NSocket *NNetwork::get_by_id(unsigned int n)
{
 for(unsigned int i = 0; i < sockets.size(); i++)
 {
         
  if(sockets[i]->socketid == n)
  {
   return sockets[i];
  }
 }
 return NULL;
}


NSocket *NNetwork::add()
{
    //cout << "NNetwork: add, Adding socket" << endl;

    NSocket *s = new NSocket();

    if(!socket_init(s) || !socket_fixreuse(s) || !socket_addlist(s))
    {
        delete s;
        return false;
    }
    
    return s;
}


NSocket *NNetwork::add(NSocket *s_i)
{
    //cout << "NNetwork: add (incomming connection)" << endl;

    if(s_i != NULL && s_i->is_listening())
    {
        struct sockaddr_in theirstuff;
        socklen_t theirstuff_len = sizeof(struct sockaddr_in); 

        int sid = -1;
    
        //cout << s_i->socketid;

        if((sid = accept(s_i->socketid, (struct sockaddr *)&theirstuff, &theirstuff_len)) == -1)
        {
            //cout << "NNetwork: add (incomming connection), failed for some reason (" << sid << ", " << errno << ")" << endl;
            perror("accept");
            return NULL;
        }

        NSocket *s = new NSocket(sid);

        if(!socket_fixreuse(s))
        {
           delete s;
           return NULL;
        }


        getpeername(sid, (struct sockaddr *)&theirstuff, &theirstuff_len);  
  
        // Add ip and port info:
        s->ip    = inet_ntoa(theirstuff.sin_addr);
        s->port  = ntohs(theirstuff.sin_port);
  
        
        if(!socket_addlist(s))
        {
            delete s;
            return NULL;
        }

        s_i->gotsome = false;

        return s; 
    }
    //cout << "Not really.. wonder why?" << endl;
    return NULL;
}



string NNetwork::host_to_address(string host)
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
 
 string address; 
 address = inet_ntoa(*((struct in_addr *)h->h_addr));
       
 return address;       
}

string NNetwork::address_to_host(string address)
{
 struct in_addr addr;
 addr.s_addr = inet_addr(address.c_str());
 
 struct hostent *h;      
 if((h = gethostbyaddr((char *)&addr.s_addr, sizeof(addr.s_addr), AF_INET)) == NULL)
 {
  return NULL;
 } 
 return (string)h->h_name;
}


NSocket *NNetwork::operator[](int n)
{
    return get_by_n(n);
}


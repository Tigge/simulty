#include "NLNetwork.hpp"


bool NLNetwork::init = false;
NLNetwork::NLNetwork()
{
 //cout << "NLNetwork: Constructor" << endl;
 if(!init)
 {
 //cout << "NLNetwork: Init first time" << endl;

  #ifdef WIN32  
   WSADATA data;
   WSAStartup(MAKEWORD(2, 2), &data);  
  #endif          
  
  init = true;
 }                 
 
 FD_ZERO(&filedescset);
 filedescmax = 0;
 
}

NLNetwork::~NLNetwork()
{
 //cout << "NLNetwork: Destructor" << endl;

 #ifdef WIN32  
  WSADATA data;
  WSAStartup(MAKEWORD(2, 2), &data);  
 #endif                         


 for(int i = sockets.size() - 1; i >= 0; i--)
 {
  //cout << "NLNetwork: Deleting socket " << i << endl;
  delete sockets[i];
  sockets.pop_back();

 } 
}


bool NLNetwork::socket_init(NLSocket *s)
{
    int res = socket(PF_INET, SOCK_STREAM, 0);
    if(res  == -1)
    {
        //cout << "NLNetwork: socket_init, failed to init socket - " << h_errno << endl;
        return false;
    }
    s->socketid = res;
    return true;
}

bool NLNetwork::socket_fixreuse(NLSocket *s)
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
        //cout << "NLNetwork: socket_fixreuse, setsockopt failed - " << errno << endl;
        return false;
    }
    return true;
}

bool NLNetwork::socket_addlist(NLSocket *s)
{
    //cout << "NLNetwork: socket_add, successful (got id " << s->socketid << ")" << endl;
    sockets.push_back(s);

    // Adjust the socket description set (max and set):
    if(s->socketid > filedescmax)filedescmax = s->socketid;
    FD_SET(s->socketid, &filedescset);

    return true;
}


bool NLNetwork::del_by_n(unsigned int n)
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

bool NLNetwork::del_by_id(unsigned int n)
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

int NLNetwork::count()
{
 ////cout << "NLNetwork: Socket count is " << sockets.size() << endl;
 return sockets.size();
}

bool NLNetwork::update(int uwait)
{
    //cout << "NLNetwork: Update" << endl;
    fd_set read  = filedescset;
    fd_set write = filedescset;

    //cout << "NLNetwork: File desc max is " << filedescmax << endl;

    struct timeval tv;
    tv.tv_sec = uwait / 1000;
    tv.tv_usec = uwait % 1000;

    int count = 0;
    if((count = select(filedescmax + 1, &read, &write, NULL, &tv)) == -1)
    {
        //cout << "NLNetwork: Update, select failed " << errno << endl;
        return false;
    }

    //cout << "NLNetwork: Update, # " << count << " sockets had an update" << endl;

    for(unsigned int i = 0, c = 0; i <= filedescmax && c < (unsigned int)count; i++)
    {
        if(FD_ISSET(i, &read) || FD_ISSET(i, &write))
        {
            NLSocket *s = get_by_id(i);
            //cout << "NLNetwork: Update. An update has hapend on socket with id" << i << endl;            
            
            c++;
            if(FD_ISSET(i, &read))
            {
              
                if(s->listening)
                {
                    //cout << "NLNetwork: Update. We got an incomming connection on socket with id" << i << endl;
                    s->gotsome = true;
                }
                else if(s->connected)
                {
                    char buf[255];
                    int count = recv(s->socketid, buf, 255, 0);

                    if(count == 0)
                    {
                        //cout << "NLNetwork: Update. A socket " << i << " disconnected gracefully" << endl;
	    
                        shutdown(s->socketid, 2);
                        s->connected = false;     
                        FD_CLR(s->socketid, &filedescset);
                    }
                    else if(count == -1)
                    {
                        //cout << "NLNetwork: Update. A socket " << i << " errored out, disconnecting" << endl;
                        shutdown(s->socketid, 2);
                        s->connected = false; 
                        FD_CLR(s->socketid, &filedescset);
                    }
                    else
                    {
                        //cout << "NLNetwork: update. Socket has something to recieve" << endl;
                        //buf[count] = '\0';
                        //cout << "NLNetwork: Update. Got " << count << " chars on socket " << i << endl;
                        //cout << "NLNetwork: Update. We got: '" << buf << "'" << endl;
                        //cout << "NLNetwork: Update. Buffer before: '" << sockread->buffer << "'" << endl;
                        //sockread->buffer += buf;
                        for(int i = 0; i < count; i++)
                        {
                            s->buffer_in.push_back(buf[i]);
                        }
                        //cout << " - we got " << count << " bytes" << endl;	                 
                        //cout << "NLNetwork: update, buffert is now '" << (char *)&(sockread->buffer[0]) << "' (" << sockread->buffer.size() << ") bytes long" << endl;	
                        //cout << "NLNetwork: Update. Buffer after: '" << sockread->buffer << "'" << endl;
                    }
                }
            }
            else if(FD_ISSET(i, &write))
            {
                if(s->connected && !s->buffer_out.empty())
                {   
                    //cout << "NLNetwork: update. Socket has something to send" << endl;
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
                  
NLSocket *NLNetwork::get_by_n(unsigned int n)
{
    if(n >= 0 && n < sockets.size())
    {
        //cout << "NLNetwork: get_by_n, found with " << n << endl;
        return sockets[n];
    }
    else
    {
        return NULL;
    }
}

NLSocket *NLNetwork::get_by_id(unsigned int n)
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


NLSocket *NLNetwork::add()
{
    //cout << "NLNetwork: add, Adding socket" << endl;

    NLSocket *s = new NLSocket();

    if(!socket_init(s) || !socket_fixreuse(s) || !socket_addlist(s))
    {
        delete s;
        return false;
    }
    
    return s;
}


NLSocket *NLNetwork::add(NLSocket *s_i)
{
    //cout << "NLNetwork: add (incomming connection)" << endl;

    if(s_i != NULL && s_i->is_listening())
    {
        struct sockaddr_in theirstuff;
        socklen_t theirstuff_len = sizeof(struct sockaddr_in); 

        int sid = -1;
    
        //cout << s_i->socketid;

        if((sid = accept(s_i->socketid, (struct sockaddr *)&theirstuff, &theirstuff_len)) == -1)
        {
            //cout << "NLNetwork: add (incomming connection), failed for some reason (" << sid << ", " << errno << ")" << endl;
            perror("accept");
            return NULL;
        }

        NLSocket *s = new NLSocket(sid);

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



std::string NLNetwork::host_to_address(std::string host)
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

std::string NLNetwork::address_to_host(std::string address)
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


NLSocket *NLNetwork::operator[](int n)
{
    return get_by_n(n);
}


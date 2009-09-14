/*
 * Server.cpp
 *
 */
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#include "Server.hpp"
#include "Exception.hpp"

using namespace std;
using namespace VCSrv;


namespace VCSrv
{


Server::Server(const string &listenAddr, unsigned short port):
            _fd(-1)
{
  _connectionString =listenAddr;
  _connectionString+=":";
  char portString[6];               // temporary...
  sprintf(portString, "%d", port);  // for this!
  _connectionString+=portString;

  sockaddr_in addr;     // our listen-on address

  addr=resolveHost(listenAddr, port);

  _fd=socket(AF_INET, SOCK_STREAM, 0);
  if(_fd==-1)
    throw Exception("Server::Server(): socket() failed");

  // bind address with socket
  if( bind(_fd, (sockaddr*)&addr, sizeof(addr) )==-1 )
  {
    close(_fd);
    throw Exception("Server::Server(): bind() failed");
  };

  int listenQueueLen=1;     // we process only one host in most cases...
  if( listen(_fd, listenQueueLen)==-1 )
  {
    close(_fd);
    throw Exception("Server::Server(): listen() failed");
  };
};


Server::~Server()
{
  close(_fd);
};


string Server::getConnectionString(void) const
{
  return _connectionString;
};


Connection Server::waitForConnection(unsigned int timeout)
{
  // wait for connection
  struct timeval  tv    ={timeout, 0};
  struct timeval *tv2use=(timeout==0)?NULL:&tv;

  for(;;)
  {
    fd_set fds;
    int    ret;

    // wait on select for data
    FD_ZERO(&fds);
    FD_SET(_fd, &fds);
    ret=select(_fd+1, &fds, NULL, NULL, tv2use);

    // error?
    if(ret==-1)
    {
      if(errno==EINTR)
        continue;
      else
        throw Exception("Server::waitForConnection(): select() failed");
    };

    // timeout?
    if(ret==0)
      throw Exception("Server::waitForConnection(): timeout");

    // has connection? accept it!
    break;
  }; // for(;;)

  sockaddr_in addrRem;
  int         addrLen=sizeof(addrRem);
  Connection  conn( accept(_fd, reinterpret_cast<sockaddr*> (&addrRem),
                                reinterpret_cast<socklen_t*>(&addrLen) ) );
  return conn;
};


sockaddr_in Server::resolveHost(const string &listenAddr, unsigned short port)
{
  sockaddr_in  addrOut;
  const char  *addr=listenAddr.c_str();

  if( strcmp(addr, "*")==0 ) // all addresses?
    addrOut.sin_addr.s_addr=INADDR_ANY;
  else
  {
    addrOut.sin_addr.s_addr=inet_addr(addr);
    if(addrOut.sin_addr.s_addr==INADDR_NONE)   // non-IP?
    {
      // if everyting fails - try DNS:
      struct hostent *dns_info=gethostbyname(addr);
      if(dns_info==NULL || dns_info->h_addr_list[0]==NULL)   // no match?
        throw Exception("Server::resolveHost(): unknown host");
      addrOut.sin_addr.s_addr=
        *((in_addr_t*)dns_info->h_addr_list[0]);   // we choose first
    };
    // so this is a correct address!
  }; // else(addr=="*")

  // rest of struct is done here:
  addrOut.sin_family=AF_INET;
  addrOut.sin_port  =htons(port);

  return addrOut;
};


}; // namespace VCSrv


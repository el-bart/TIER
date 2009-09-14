/*
 * TCPServer.cpp
 *
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>

#include "TCPServer.hpp"
#include "Resolver.hpp"


namespace Network
{

TCPServer::TCPServer(const Address &addr, int listenQueueLen):
                Server(addr)
{
  const Address ipPort=Resolver::resolve(addr);

  sockaddr_in sockAddr;     // our listen-on address
  sockAddr.sin_addr.s_addr=inet_addr( ipPort.getHost().c_str() );
  assert(sockAddr.sin_addr.s_addr!=INADDR_NONE);
  sockAddr.sin_family     =AF_INET;
  sockAddr.sin_port       =ipPort.getPortHTON();

  const int fd=socket(AF_INET, SOCK_STREAM, 0);
  if(fd==-1)
    throw Exception("TCPServer::TCPServer(): socket() failed");

  // bind address with socket
  if( bind(fd, reinterpret_cast<sockaddr*>(&sockAddr),
           sizeof(sockAddr) )==-1 )
  {
    close(fd);
    throw Exception("TCPServer::TCPServer(): bind() failed");
  };

  // begin listening on the address
  if( listen(fd, listenQueueLen)==-1 )
  {
    close(fd);
    throw Exception("TCPServer::TCPServer(): listen() failed");
  };

  //
  // write down our socket info!
  //
  setSock(fd);
};


Connection TCPServer::accept(void)
{
  int fd=_sock.get()->getSock();    // helper

  // accept and return connection
  sockaddr_in addrRem;
  int         addrLen=sizeof(addrRem);
  return connFromSock( ::accept(fd, reinterpret_cast<sockaddr*> (&addrRem),
                                    reinterpret_cast<socklen_t*>(&addrLen) ) );
};

}; // namespace Network


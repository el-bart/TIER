/*
 * TCPClient.cpp
 *
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#include "TCPClient.hpp"
#include "Resolver.hpp"


namespace Network
{

TCPClient::TCPClient(const Address &addr):
            Client(addr)
{
  Address ipPort=Resolver::resolve(addr);

  sockaddr_in sockAddr;     // our connection address

  sockAddr.sin_addr.s_addr=inet_addr( ipPort.getHost().c_str() );
  assert(sockAddr.sin_addr.s_addr!=INADDR_NONE);
  sockAddr.sin_family     =AF_INET;
  sockAddr.sin_port       =ipPort.getPortHTON();

  const int fd=socket(AF_INET, SOCK_STREAM, 0);
  if(fd==-1)
    throw Exception("TCPClient::TCPClient(): socket() failed");

  // connect to remote host
  if( connect(fd, reinterpret_cast<const sockaddr*>(&sockAddr),
              sizeof(sockAddr) )==-1 )
  {
    close(fd);
    throw Exception("TCPClient::TCPClient(): connect() failed");
  };

  // set out connection
  try
  {
    setConn(fd);
  }
  catch(...)
  {
    close(fd);
    throw;
  };

};

}; // namespace Network


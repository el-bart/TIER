/*
 * Server.hpp
 *
 * abstract class representing server side of
 * the connection.
 *
 */
#ifndef INCLUDE_NETWORK_SERVER_HPP_FILE
#define INCLUDE_NETWORK_SERVER_HPP_FILE

#include <memory>
#include <assert.h>

#include "Network/Socket.hpp"
#include "Network/Connection.hpp"
#include "Network/Address.hpp"
#include "Network/Exception.hpp"


namespace Network
{

class Server
{
public:
  inline const Network::Address &getAddress(void) const
  {
    return _addr;
  };

  inline bool isActive(void) const
  {
    return _sock->isActive();
  };

  // checks if connection is aviable and returns true/false.
  //   ms - timeout for waiting in milliseconds (0==infinity)
  bool waitForConnection(unsigned int ms=0);

protected:
  Server(const Network::Address &addr):
    _addr(addr)
  {
  };

  // this is intended tu be used by derived classes
  inline void setSock(int sock)
  {
    assert( _sock.get()==NULL );        // this should be done only once!
    _sock.reset( new Network::Socket(sock) );
  };

  // makes connection from socket (file descriptor)
  inline Network::Connection connFromSock(int sock) const
  {
    Network::Socket socket(sock);
    return Network::Connection(socket);
  };

  std::auto_ptr<Network::Socket> _sock; // open-port socket
  const Network::Address         _addr; // listen address
};

}; // namespace Network

#endif


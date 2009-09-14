/*
 * Client.hpp
 *
 * abstract class representing client side of
 * the connection.
 *
 */
#ifndef INCLUDE_NETWORK_CLIENT_HPP_FILE
#define INCLUDE_NETWORK_CLIENT_HPP_FILE

#include <memory>
#include <assert.h>

#include "Network/Connection.hpp"
#include "Network/Address.hpp"
#include "Network/Exception.hpp"


namespace Network
{

class Client
{
public:
  inline const Network::Address &getAddress(void) const
  {
    return _addr;
  };

  // we return pointer instead of reference,
  // beacause reference could be accidently "captured"
  // by non-reference, deactualizing our internal instance.
  inline Network::Connection *getConn(void) const
  {
    if( !_conn->isActive() )
      throw Network::Exception("Connection::getConn(): not connected");
    return _conn.get();
  };

  inline bool isActive(void) const
  {
    return _conn->isActive();
  };

  inline void flush(void)
  {
    _conn->flush();
  };
  inline void flushRX(void)
  {
    _conn->flushRX();
  };
  inline void flushTX(void)
  {
    _conn->flushTX();
  };

protected:
  Client(const Network::Address &addr):
    _addr(addr)
  {
  };

  inline void setConn(int sock)             // sets connection from sock
  {
    assert(0<=sock);
    Network::Socket socket(sock);           // temporary socket
    assert( _conn.get()==NULL );            // this can be done just once!
    _conn.reset( new Network::Connection(socket) ); // write connection
  };

  std::auto_ptr<Network::Connection> _conn; // out connection
  const Network::Address             _addr; // address we're connected with
};

}; // namespace Network

#endif


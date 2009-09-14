/*
 * Client.hpp
 *
 * class providing network functionality for image
 * recieveing component.
 *
 */
#ifndef INCLUDE_VCCLN_CLIENT_HPP_FILE
#define INCLUDE_VCCLN_CLIENT_HPP_FILE

#include <string>
#include <netinet/in.h>

#include "Network/Connection.hpp"
#include "Network/TCPClient.hpp"
#include "VCCln/Exception.hpp"


namespace VCCln
{

class Client: public Network::TCPClient
{
public:
  //
  // srvAddr - address (IP/name/*) to connect to
  // port    - port on server to connect to
  //
  Client(const Network::Address &addr):
        TCPClient(addr)
  {
  };

  // returns "addr:port" string
  std::string getConnectionString(void) const
  {
    return getAddress().getAddressStr();
  };

  Network::Connection *getConnection(void) const
  {
    return _conn.get();
  };

  void sendRequest(void)
  {
    char req='!';
    if( fwrite( &req, 1, 1, _conn->getTXstrm() )!=1 )
      throw VCCln::Exception("Client::sendRequest(): fwrite() failed");
    flush();
  };
};

}; // namespace VCCln


#endif


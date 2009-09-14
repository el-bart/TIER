/*
 * Server.hpp
 *
 * class providing network functionality for image sending
 * component.
 *
 */
#ifndef INCLUDE_VCSRV_SERVER_HPP_FILE
#define INCLUDE_VCSRV_SERVER_HPP_FILE

#include <string>
#include <netinet/in.h>

#include "VCSrv/RasterSerializer.hpp"
#include "VCSrv/Connection.hpp"
#include "VCSrv/Exception.hpp"


namespace VCSrv
{

class Server
{
public:
  //
  // listenAddr - address (IP/name/*) to listen on
  // port       - port to listen for conecntions on
  //
  Server(const std::string &listenAddr, unsigned short port);
  ~Server();

  // returns "addr:port" string
  std::string getConnectionString(void) const;

  //
  // waits for connection from foregin host and returns
  // connection description class if connection spoted or
  // throw if timeout occures.
  //   timeout - in [s] or infinit for value of 0
  //
  VCSrv::Connection waitForConnection(unsigned int timeout);

private:

  // throws on resolv error
  sockaddr_in resolveHost(const std::string &listenAddr, unsigned short port);

  std::string _connectionString;
  int         _fd;
};

}; // namespace VCSrv


#endif


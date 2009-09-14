/*
 * TCPServer.hpp
 *
 * class representing server side of
 * the TCP connection.
 *
 */
#ifndef INCLUDE_NETWORK_TCPSERVER_HPP_FILE
#define INCLUDE_NETWORK_TCPSERVER_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Network/Server.hpp"
#include "Network/Connection.hpp"
#include "Network/Exception.hpp"


namespace Network
{

class TCPServer: public  Network::Server,
                 private boost::noncopyable
{
public:
  // open TCP port for incomming connections on a given
  // ip:port
  //   listenQueueLen - how many host may be in queue at a time?
  TCPServer(const Network::Address &addr,
            int listenQueueLen=3);

  // accept and return connection.
  Network::Connection accept(void);
};

}; // namespace Network

#endif


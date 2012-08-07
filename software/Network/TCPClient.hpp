/*
 * TCPClient.hpp
 *
 * client side of TCP connection.
 *
 */
#ifndef INCLUDE_NETWORK_TCPCLIENT_HPP_FILE
#define INCLUDE_NETWORK_TCPCLIENT_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Network/Client.hpp"
//#include "Network/

namespace Network
{

class TCPClient: public  Network::Client,
                 private boost::noncopyable
{
public:
  // connects with a given host
  TCPClient(const Network::Address &addr);
};

};

#endif


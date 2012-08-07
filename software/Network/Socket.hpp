/*
 * Socket.hpp
 *
 */
#ifndef INCLUDE_NETWORK_SOCKET_HPP_FILE
#define INCLUDE_NETWORK_SOCKET_HPP_FILE

#include <unistd.h>

#include "Network/Exception.hpp"


namespace Network
{

// forward declarations
class Server;
class Server;
class Client;

//
// class representing the socket.
// note: this class does NOT behave like a value!
//
class Socket
{
public:
  Socket(const Network::Socket &s)
  {
    _sock=-1;       // no socket asociated yet!
    moveSocket(s);
  };
  ~Socket(void)
  {
    close();
  };

  Network::Socket operator=(Network::Socket &s)
  {
    moveSocket(s);
    return s;       // doesn't make any sense, but must be done...
  };

  inline int getSock(void) const
  {
    return _sock;
  };

  inline bool isActive(void) const
  {
    return _sock!=-1;
  };

private:
  // class takes over the socket - do NOT use or close
  // it at ANY circumstances.
  Socket(int sock):
    _sock(sock)
  {
    if(_sock<0)
      throw Network::Exception("Socket::Socket(): socket value is incorrect");
  };

  inline void moveSocket(const Network::Socket &s)
  {
    close();            // just in case :)
    _sock  =s._sock;    // we move socket to us...
    s._sock=-1;         // ... and take ovnership!
  };

  inline void close(void)
  {
    ::close(_sock);     // here we use standard C function close()
    _sock=-1;
  };

  // this will be needed, so that only these classes could
  // estabilish connection:
  friend class Network::Server;
  friend class Network::Client;

  mutable int _sock;    // our socket
};

}; // namespace Network

#endif


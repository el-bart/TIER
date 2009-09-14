/*
 * Connection.hpp
 *
 * class holding parameters for a single connection.
 *
 */
#ifndef INCLUDE_NETWORK_CONNECTION_HPP_FILE
#define INCLUDE_NETWORK_CONNECTION_HPP_FILE

#include <stdio.h>
#include <unistd.h>

#include "Network/Socket.hpp"
#include "Network/Exception.hpp"


namespace Network
{

//
// class representing single connection. it can be copied
// thou it does NOT behave like a value (just like auto_ptr).
//
class Connection
{
public:
  //
  // sock - socket of a connection to hold. after creating
  //        object of this class it **owns** sock socket!
  //
  Connection(Network::Socket &sock):
    _sock( sock )
  {
    const int &fd=_sock.getSock();  // just a helper

    _rx=fdopen( dup(fd), "r");
    if(_rx==NULL)
    {
      close();
      throw Network::Exception("Network::Connection(): "
                               "unable to create RX stream");
    };

    _tx=fdopen( dup(fd), "w");
    if(_tx==NULL)
    {
      close();
      throw Network::Exception("Network::Connection(): "
                               "unable to create TX stream");
    };
  };

  Connection(const Network::Connection &conn):
    _sock( conn._sock )
  {
    moveStreams(conn);
  };

  ~Connection()
  {
    close();
  };

  Connection operator=(Network::Connection &conn)
  {
    moveConnection(conn);
    return conn;            // doesn't do anything but must be done
  };

  // returns pointer to proper transmition stream
  // (do NOT close these streams manualy!):
  inline FILE *getRXstrm(void)
  {
    return _rx;
  };
  inline FILE *getTXstrm(void)
  {
    return _tx;
  };

  inline bool isActive(void) const
  {
    return _rx!=NULL && _tx!=NULL && _sock.isActive();
  };

  // it is sometimes required to flush buffers
  inline void flush(void)
  {
    flushRX();
    flushTX();
  };
  inline void flushRX(void)
  {
    fflush(_rx);
  };
  inline void flushTX(void)
  {
    fflush(_tx);
  };

private:
  inline void moveConnection(const Network::Connection &conn)   // helper
  {
    close();                        // just in case...
    _sock=conn._sock;               // owner ship is taken
    moveStreams(conn);              // move rest...
  };

  inline void moveStreams(const Network::Connection &conn)      // helper
  {
    // copy pointers
    _rx  =conn._rx;
    _tx  =conn._tx;

    // take the ownerships
    conn._rx=NULL;
    conn._tx=NULL;
  };

  inline void close(void)           // helper
  {
    // _sock is closed automaticaly when destroyed
    if(_rx!=NULL)
      fclose(_rx);
    if(_tx!=NULL)
      fclose(_tx);
  };

  mutable Network::Socket  _sock;   // base socket
  mutable FILE            *_rx;     // RX stream
  mutable FILE            *_tx;     // TX stream
};

}; // namespace VCCln

#endif


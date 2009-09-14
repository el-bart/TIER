/*
 * Connection.hpp
 *
 * class holding parameters for a single connection.
 *
 */
#ifndef INCLUDE_VCSRV_CONNECTION_HPP_FILE
#define INCLUDE_VCSRV_CONNECTION_HPP_FILE

#include <stdio.h>

#include "VCSrv/Exception.hpp"


namespace VCSrv
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
  Connection(int sock);
  Connection(const VCSrv::Connection &conn);
  ~Connection();

  Connection operator=(VCSrv::Connection &conn);

  //
  // returns true if got request before timeout.
  // if timeout is 0 then waits forever.
  //   timeout - how log to wait for request [s].
  //
  bool waitForRequest(unsigned int timeout);
  // returns pointer to transmition stream
  FILE *getTXstream(void);

private:
  void mvConn(const VCSrv::Connection &conn); // helper
  void freeSelf(void);                        // helper

  mutable int   _sock;
  mutable FILE *_tx;
};

}; // namespace VCSrv

#endif


/*
 * Connection.cpp
 *
 */
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include <assert.h>

#include "Connection.hpp"
#include "Exception.hpp"


using namespace std;


namespace VCSrv
{


Connection::Connection(int sock):
        _sock(sock)
{
  if(sock<3)        // std I/O is not allowed here (0,1,2)
    throw Exception("Connection::Connection(): bad sock number (sock<3)");

  _tx=fdopen( dup(_sock), "w" );
  if(_tx==NULL)
  {
    close(_sock);
    throw Exception("Connection::Connection(): connecting socket with "
                    "stream failed");
  };
};


Connection::Connection(const Connection &conn)
{
  mvConn(conn);
};


Connection::~Connection()
{
  freeSelf();
};


Connection Connection::operator=(Connection &conn)
{
  mvConn(conn);
  return conn;      // stupid, but must be done...
};


bool Connection::waitForRequest(unsigned int timeout)
{
  struct timeval  tv   ={timeout, 0};
  struct timeval *tvPtr=(timeout>0)?&tv:NULL;   // wait until infinit?

  for(int quit=0; !quit;)
  {
    fd_set fds;
    int    ret;

    // wait on select for data
    FD_ZERO(&fds);
    FD_SET(_sock, &fds);
    ret=select(_sock+1, &fds, NULL, NULL, tvPtr);

    // error?
    if(ret==-1)
    {
      if(errno==EINTR)
        continue;
      else
        throw Exception("Connection::waitForRequest(): select() failed");
    };

    // timeout?
    if(ret==0)
      return false;

    // has data? read data!
    char tmp;
    switch( read( _sock, &tmp, sizeof(tmp) ) )
    {
      case 0:
        throw Exception("Connection::waitForRequest(): connectoin closed");
        break;

      case sizeof(tmp):
        quit=1;
        break;

      case -1:
      default:
        throw Exception("Connection::waitForRequest(): read() failed");
        break;
    };
  }; // for(;;)

  return true;
};


FILE *Connection::getTXstream(void)
{
  return _tx;
};


void Connection::freeSelf(void)
{
  if(_tx!=NULL)
  {
    fclose(_tx);
    _tx=NULL;
  };

  close(_sock);
  _sock=-1;
};


void Connection::mvConn(const Connection &conn)
{
  // just-in-case free our resources
  freeSelf();
  // take ovnership
  _sock=conn._sock;
  _tx  =conn._tx;
  // remove it from copied object
  conn._sock=-1;
  conn._tx  =NULL;
};


}; // namespace VCSrv


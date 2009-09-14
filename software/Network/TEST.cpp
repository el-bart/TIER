/*
 * TEST.cpp
 *
 */

#include <iostream>
#include <memory>

#include "Exception.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Connection.hpp"
#include "Address.hpp"
#include "Resolver.hpp"
#include "TCPServer.hpp"
#include "TCPClient.hpp"


using namespace std;
using namespace Network;



int main(int argc, const char * const * const argv)
{
  try
  {
    for(int i=0; i<9; ++i)
    {
      Address   addr("127.0.0.1", 2002);
      TCPServer srv(addr);
      std::auto_ptr<Connection> aSrvC;

      {
        TCPClient cln(addr);

        assert( srv.waitForConnection(1000) );
        Connection conn=srv.accept();
        aSrvC.reset( new Connection(conn) );
      }

      sleep(1);
      cerr<<"#";
    }; // for(i)
    cerr<<endl;
  }
  catch(Network::Exception &ex)
  {
    cerr<<argv[0]<<": Network::Exception: "<<ex.what()<<endl;
    return 1;
  };
  return 0;
}; // main()


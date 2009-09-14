/*
 * ServerApp.cpp
 *
 * main source file, providing network interface for
 * image capturing from Video4Linux video devices on
 * demand.
 * to recive lastly captured image you need to send
 * single byte of any value to a server.
 *
 */
#include <iostream>
#include <stdlib.h>
#include <signal.h>

#include "RasterSerializer.hpp"
#include "Exception.hpp"
#include "Graphics/V4LCapture.hpp"
#include "Network/TCPServer.hpp"

using namespace std;
using namespace VCSrv;
using namespace Graphics;
using namespace Network;



//
// MAIN
//
int main(int argc, const char * const argv[])
{
  int ret=0;        // application return code

  // got nessesari arguments?
  if(argc!=6)
  {
    cerr<<argv[0]<<" <listen_addr> <port> <v4l_dev> <size_x> <size_y>"<<endl;
    return ret=1;
  };

  // we need to ignore some signals:
  signal(SIGPIPE, SIG_IGN);

  try
  {

    string v4lDev(argv[3]);
    Size   size( atoi(argv[4]), atoi(argv[5]) );
    V4LCapture cap(v4lDev, size);           // create capturing interface
    Address    addr(argv[1], atoi(argv[2]) );   // listen address
    TCPServer  srv(addr);                   // open for incomming connections
    cout<<argv[0]<<": listening @ "<<srv.getAddress().getAddressStr()<<endl;

    //
    // main loop
    //
    for(;;)
    {
      Connection       cln=srv.accept();        // get new connection
      RasterSerializer rSer( cln.getTXstrm() ); // data sending channel

      // wait for requests and send data
      try
      {
        char c;
        //while( cln.waitForRequest(0) )        // send new frame?
        while( fread(&c, 1, 1, cln.getRXstrm() ) )       // send new frame?
        {
          RasterRGB::AutoRaster r=cap.capture(0);        // capture 1 frame
          rSer.serialize( *r.get() );           // send raster
        }; // while(capture_request)
      }
      catch(const Graphics::Exception &e)
      {
        cerr<<argv[0]<<": Graphics Exception: "<<e.what()<<endl;
        cerr<<argv[0];
        perror(": perror");
      }
      catch(const VCSrv::Exception &e)
      {
        // connection closed. :)
        cerr<<argv[0]<<": VCSrv Exception: "   <<e.what()<<endl;
        cerr<<argv[0];
        perror(": perror");
      };
    }; // for(main_loop)

  }
  catch(const Graphics::Exception &e)
  {
    cerr<<argv[0]<<": Graphics Exception: "<<e.what()<<endl;
    cerr<<argv[0];
    perror(": perror");
    ret=11;
  }
  catch(const VCSrv::Exception &e)
  {
    cerr<<argv[0]<<": VCSrv Exception: "   <<e.what()<<endl;
    cerr<<argv[0];
    perror(": perror");
    ret=10;
  }
  catch(const Network::Exception &e)
  {
    cerr<<argv[0]<<": Network Exception: "   <<e.what()<<endl;
    cerr<<argv[0];
    perror(": perror");
    ret=10;
  }
  catch(...)
  {
    cerr<<argv[0]<<": Unknown exception"<<endl;
    cerr<<argv[0];
    perror(": perror");
    ret=3;
  };

  return ret;
}; // main()


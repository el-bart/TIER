/*
 * TEST.cpp
 *
 * ClientTESTApp
 *
 * test source file, providing network interface for
 * image capturing from Video4Linux Server on demand.
 * to recive lastly captured image you need to send
 * single byte of any value to a server.
 *
 */
#include <iostream>
#include <stdlib.h>

#include "CGraphics/Raster.hpp"
#include "CGraphics/Exception.hpp"
#include "Client.hpp"
#include "RasterDeserializer.hpp"
#include "Exception.hpp"

using namespace std;
using namespace VCCln;
using namespace CGraphics;
using namespace Network;



//
// MAIN
//
int main(int argc, const char * const argv[])
{
  int ret=0;        // application return code

  // got nessesari arguments?
  if(argc!=3)
  {
    cerr<<argv[0]<<" <srv_addr> <port>"<<endl;
    return ret=1;
  };

  try
  {

    Address       addr(argv[1], atoi(argv[2]) );
    VCCln::Client cln(addr);         // connect
    cout<<argv[0]<<": connecting to "<<cln.getConnectionString()<<endl;
    Connection *conn=cln.getConnection();             // get connection
    RasterDeserializer rstDsrl( conn->getRXstrm() );  // mk deserializer
    cout<<argv[0]<<": starting raster(s) capture..."<<endl;

    //
    // main loop
    //
    for(unsigned int i=0; /*i<350*/; ++i)
    {
      cln.sendRequest();
      Raster::AutoRaster r=rstDsrl.deserialize();
      cerr<<".";
      char fileName[100];
      sprintf(fileName, "/tmp/vccln_out_%010u.bmp", i);
      r->toFile(fileName);
    }; // for(main_loop)

  }
  catch(const CGraphics::Exception &e)
  {
    cerr<<argv[0]<<": CGraphics Exception: "<<e.what()<<endl;
    cerr<<argv[0];
    perror(": perror");
    ret=11;
  }
  catch(const VCCln::Exception &e)
  {
    cerr<<argv[0]<<": VCCln Exception: "   <<e.what()<<endl;
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


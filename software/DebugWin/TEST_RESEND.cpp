/*
 * TEST.cpp
 *
 */

#include <iostream>

#include "VCCln/Client.hpp"
#include "VCCln/RasterDeserializer.hpp"
#include "VCCln/Exception.hpp"
#include "VCSrv/Exception.hpp"
#include "DebugTransmit.hpp"

using namespace std;
using namespace Network;
using namespace DebugWin;
using namespace CGraphics;
using namespace VCCln;


int main(const int argc, char const * const * const argv)
{
  try
  {
    if(argc!=1+2+2)
    {
      cerr<<argv[0]<<" <ipWin> <portWin>  <ipSrv> <portSrv>"<<endl;
      return 1;
    };

    Address addrWin(argv[1], atoi(argv[2]) );
    DebugTransmit transmit(addrWin);

    // address of recieve-from computer
    Address addrSrv(argv[3], atoi(argv[4]) );
    VCCln::Client  cln(addrSrv);
    Connection    *conn=cln.getConn();

    RasterDeserializer rds( conn->getRXstrm() );

    cerr<<"re-sending: ";
    while(1)
    {
      try
      {
        cerr<<"#";
        cln.sendRequest();
        CGraphics::Raster::AutoRaster aRaster=rds.deserialize();
        transmit.debug( *aRaster.get() );   // send image
      }
      catch(...)
      {
        cerr<<">E<";
      };
    };
    cerr<<endl;
  }
  catch(const CGraphics::Exception &ex)
  {
    cerr<<argv[0]<<": CGraphics exception: "<<ex.what()<<endl;
  }
  catch(const VCCln::Exception &ex)
  {
    cerr<<argv[0]<<": VCCln exception: "<<ex.what()<<endl;
  }
  catch(const VCSrv::Exception &ex)
  {
    cerr<<argv[0]<<": VCSrv exception: "<<ex.what()<<endl;
  }
/*  catch(const Graphics::Exception &ex)
  {
    cerr<<argv[0]<<": Graphics exception: "<<ex.what()<<endl;
  }*/
  catch(const Network::Exception &ex)
  {
    cerr<<argv[0]<<": Network exception: "<<ex.what()<<endl;
  }
/*  catch(const Threads::Exception &ex)
  {
    cerr<<argv[0]<<": Threads exception: "<<ex.what()<<endl;
  }*/
  catch(...)
  {
    cerr<<argv[0]<<": generic exception cought..."<<endl;
  };

  return 0;
}; // main()


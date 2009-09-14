/*
 * TEST.cpp
 *
 */

#include <iostream>

#include "VCCln/Exception.hpp"
#include "VCSrv/Exception.hpp"
#include "DebugTransmit.hpp"

using namespace std;
using namespace Network;
using namespace DebugWin;
using namespace CGraphics;



void process(Raster &r)
{
  for(unsigned int y=0; y<r.getSize()._y; ++y)
    for(unsigned int x=0; x<r.getSize()._x; ++x)
    {
      Position pos(x,y);
      Pixel    pix=r.getPixel(pos);

      pix.setR( pix.getR()+1 );
      pix.setG( pix.getG()+1 );
      pix.setB( pix.getB()+1 );

      r.setPixel(pos, pix);
    };
};


int main(const int argc, char const * const * const argv)
{
  try
  {
    if(argc!=1+2)
    {
      cerr<<argv[0]<<" <ip> <port>"<<endl;
      return 1;
    };

    Address addr(argv[1], atoi(argv[2]) );
    DebugTransmit transmit(addr);

    Raster::AutoRaster aRaster=Raster::fromFile("test.bmp");

    cerr<<"sending: ";
    for(int i=0; i<100; ++i)
    {
      cerr<<"#";
      process( *aRaster.get() );          // do something with raster...
      transmit.debug( *aRaster.get() );   // send image
//      usleep(125);                        // wait a while...
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


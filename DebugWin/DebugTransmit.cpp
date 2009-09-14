/*
 * DebugTransmit.cpp
 *
 */
#include <iostream>

#include "VCSrv/RasterSerializer.hpp"
#include "Network/TCPClient.hpp"
#include "DebugTransmit.hpp"


using namespace std;
using namespace Network;
using namespace Maps;
using namespace Vectors;


namespace DebugWin
{

void DebugTransmit::debug(const Graphics::RasterRGB  &r)
{
  try
  {
    TCPClient               client(_addr);  // connection or exception!
    VCSrv::RasterSerializer serializer( client.getConn()->getTXstrm() );
    serializer.serialize(r);                // send the data! :)
  }
  // this is ONLY debugger - exceptions doesn't matter here much...
  catch(const Network::Exception &ex)
  {
    cerr<<"DebugTransmit::debug(): Network exception: "<<ex.what()<<endl;
  }
  catch(const VCSrv::Exception &ex)
  {
    cerr<<"DebugTransmit::debug(): VCSrv exception: "<<ex.what()<<endl;
  }
  catch(...)
  {
    cerr<<"DebugTransmit::debug(): exception cought"<<endl;
  };
};


void DebugTransmit::debug(const CGraphics::Raster &cr)
{
  // we need to transform CGraphics::Raster -> Graphics::Raster
  const Graphics::Size size( cr.getSize()._x, cr.getSize()._y );
  Graphics::RasterRGB  r(size);

  for(unsigned int y=0; y<size.y(); ++y)
    for(unsigned int x=0; x<size.x(); ++x)
    {
      CGraphics::Position cpos(x,y);
      Graphics::Position  pos (x,y);

      CGraphics::Pixel    cpix( cr.getPixel(cpos) );
      Graphics::PixelRGB  pix( cpix.getR(), cpix.getG(), cpix.getB() );

      r.setPixel(pos, pix);
    };

  // run sender itself:
  debug(r);
};


void DebugTransmit::debug(const Maps::VectorImg &vi,
                          Graphics::Size         size,
                          Graphics::Position     pos,
                          double                 k,
                          bool                   vFlip)
{
  Graphics::RasterRGB r(size);

  const Graphics::PixelRGB black(  0,  0,  0);
  const Graphics::PixelRGB white(255,255,255);
  r.fillRaster(black);

  for(unsigned int i=0; i<vi.linesCnt(); ++i)
  {
    const Line2DCont &l =vi[i];      // get reference ot object
    Point2DCont       pF=l.getFrom();
    Point2DCont       pT=l.getTo();
    // scaling:
    pF=Point2DCont(pF[0]*k, pF[1]*k);
    pT=Point2DCont(pT[0]*k, pT[1]*k);

    Vector2DCont      dv(pF, pT);
    unsigned int      vLen=static_cast<unsigned int>( ceil( dv.length() ) );

    // draw line each time moving by ~1 point
    dv.normalize();
    for(unsigned long j=0; j<vLen; ++j)
    {
      Graphics::Position p(
          static_cast<unsigned int>( pF[0]+j*dv[0]+pos.x() ),
          static_cast<unsigned int>( pF[1]+j*dv[1]+pos.y() ) );

      if(vFlip)
        p=Graphics::Position( p.x(), size.y()-p.y() );
      r.setPixel(p, white);
    };

  }; // for(lines)

  // run sender itself
  debug(r);
};

}; // namespace DebugWin


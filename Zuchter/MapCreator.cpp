/*
 * MapCreator.cpp
 *
 */
#include <string>
#include <string.h>
#include <errno.h>

#include "MapCreator.hpp"

using namespace std;
using namespace Maps;
using namespace Path;
using namespace VCCln;
using namespace Network;


// debug code:
#include "DebugWin/DebugTransmit.hpp"
static DebugWin::DebugTransmit mkDt(std::string addr, unsigned short port)
{
  Network::Address a(addr, port);
  return DebugWin::DebugTransmit(a);
};
static DebugWin::DebugTransmit dt_0=mkDt("127.0.0.1", 1230);
static DebugWin::DebugTransmit dt_1=mkDt("127.0.0.1", 1231);
static DebugWin::DebugTransmit dt_2=mkDt("127.0.0.1", 1232);
static DebugWin::DebugTransmit dt_3=mkDt("127.0.0.1", 1233);
// end of debug code


namespace Zuchter
{

MapCreator::MapCreator(const MapCreatorParameters &p):
  _listener(p._listener),
  _size( p._dpthTransParams.getSize() ),
  _csize( _size.x(), _size.y() ),
  _cropMargin(p._cropMargin),
  _vcSrvCln( new VCCln::Client(p._addr) ),
  _rasterDeserializer( _vcSrvCln->getConnection()->getRXstrm() ),
  _segmParams(p._segmParams),
  _segment(_csize),
  _vectorizer(_csize),
  _dpthTransformer(p._dpthTransParams)
{
};


const GlobalMap &MapCreator::updateGlobalMap(void)
{
  try
  {
    Map2D::AutoMap2D amap=createLocalMap();   // make local map...
    _gMap.actualize( *amap.get() );           // update global map
                                              // from gathered info.
  }
  catch(const Vectors::Exception &e)
  {
    string err=string("MapCreator::updateGlobalMap: Vectors::") + e.what();
    _listener.onGraphicsError(err);
  }
  catch(const Maps::Exception &e)
  {
    string err=string("MapCreator::updateGlobalMap: Maps::") + e.what();
    _listener.onGraphicsError(err);
  }
  catch(const CGraphics::Exception &e)
  {
    string err=string("MapCreator::updateGlobalMap: CGraphics::") + e.what();
    _listener.onGraphicsError(err);
  }
  catch(const Graphics::Exception &e)
  {
    string err=string("MapCreator::updateGlobalMap: Graphics::") + e.what();
    _listener.onGraphicsError(err);
  }
  catch(const Network::Exception &e)
  {
    string err=string("MapCreator::updateGlobalMap: Network::") + e.what() +
               string(" / perror(): ") + string( strerror(errno) );
    _listener.onNetworkError(err);
  }
  catch(const VCCln::Exception &e)
  {
    string err=string("MapCreator::updateGlobalMap: VCCln::") + e.what();
    _listener.onNetworkError(err);
  }
  catch(const Path::Exception &e)
  {
    string err=string("MapCreator::updateGlobalMap: Path::") + e.what();
    _listener.onGraphicsError(err);
  }
  catch(...)
  {
    string err=string("MapCreator::updateGlobalMap: unknown exception; ")
               + string("perror(): ") + string( strerror(errno) );
    _listener.onGenericError(err);
  };

  return _gMap;
};


Map2D::AutoMap2D MapCreator::createLocalMap(void)
{
  // grab single raster-frame from server:
  _vcSrvCln->sendRequest();
  CGraphics::Raster::AutoRaster acr=_rasterDeserializer.deserialize();

  // imag ecropping to a given size
  {
    CGraphics::Size     size=acr->getSize();
    CGraphics::Position from(_cropMargin,         _cropMargin        );
    CGraphics::Position to  (size._x-_cropMargin+1, size._y-_cropMargin-1);
    acr=acr->crop(from, to);
  };
  if( !(acr->getSize()==_csize) )       // size mismatch?
    acr=acr->resize(_csize);            // size MUST fit...
dt_0.debug(*acr.get());   // TODO: test only

  // we have image, so we segment it:
  _segment.process(_segmParams, *acr.get() );
dt_1.debug( *acr->markerToColormap().get() );   // TODO: test only

  // and vectorize it:
  VectorImg::AutoVectorImg avi=_vectorizer.vectorize( *acr.get() );
dt_2.debug(*avi.get(), _size, Graphics::Position(0,0), 1, true);

  // now perform deepth transformation:
  avi=_dpthTransformer.transform( *avi.get() );
{
const int k=1;
Graphics::Size tmpSize(_size.x()*k, _size.y()*k);
dt_3.debug(*avi.get(), tmpSize,
Graphics::Position(tmpSize.x()/2,3), 70, true);

//sleep(2);
}

  return Map2D::AutoMap2D( new Map2D( *avi.get() ) );
};


bool MapCreator::reconnect(void)
{
  try
  {
    _vcSrvCln.reset( new VCCln::Client( _vcSrvCln->getAddress() ) );
    _rasterDeserializer=RasterDeserializer(
        _vcSrvCln->getConnection()->getRXstrm() );
  }
  catch(...)
  {
    return false;
  };
  return true;
};

}; // namespace Zuchter


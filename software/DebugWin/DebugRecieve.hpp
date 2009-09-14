/*
 * DebugReciev.hpp
 *
 * class wrapping around debugging images reciever.
 *
 */
#ifndef INCLUDE_DEBUGWIN_DEBUGRECIEV_HPP_FILE
#define INCLUDE_DEBUGWIN_DEBUGRECIEV_HPP_FILE

#include <iostream>
#include <memory>
#include <boost/noncopyable.hpp>

#include "VCCln/RasterDeserializer.hpp"
#include "Network/TCPServer.hpp"
#include "Threads/Thread.hpp"
#include "DebugWin/RasterSwapper.hpp"


namespace DebugWin
{

class DebugRecieve: public Threads::Thread
{
public:
  DebugRecieve(const Network::Address  &addr,
               DebugWin::RasterSwapper &swapper):
    _server(addr, 1),
    _swapper(swapper)
  {
  };

private:
  // helper holder for C-pointer
  struct CRasterHolder: private boost::noncopyable
  {
    CRasterHolder(struct raster *r):
      _r(r)
    {
    };
    ~CRasterHolder(void)
    {
      if(_r!=NULL)
        raster_free(_r);
    };

    struct raster *get(void)
    {
      return _r;
    };

    struct raster *release(void)
    {
      struct raster *r=_r;
      _r=NULL;
      return r;
    };

  private:
    struct raster *_r;
  };

  virtual void doTask(void)
  {
    try
    {
//std::cerr<<"doTask(): testing for connection..."<<std::endl;
      if( !_server.waitForConnection(1500) )   // wait 1.5[s] before fallback
        return;

      // so we have connection! :)
//std::cerr<<"doTask(): got connection"<<std::endl;
      Network::Connection           conn       =_server.accept();
      VCCln::RasterDeserializer     deserializer( conn.getRXstrm() );
      CGraphics::Raster::AutoRaster aRaster    =deserializer.deserialize();
      CRasterHolder                 autoCRaster( convert( *aRaster.get() ) );
      _swapper.swap( autoCRaster.get() );
      autoCRaster.release();        // pointer is secure now!
//std::cerr<<"doTask(): got new immage"<<std::endl;
    }
    catch(const Network::Exception &ex)
    {
      std::cerr<<"DebugReciever::doTask(): Network exception: "
               <<ex.what()<<std::endl;
      //cancelThread();
    }
    catch(const VCCln::Exception &ex)
    {
      std::cerr<<"DebugReciever::doTask(): VCCln exception: "
               <<ex.what()<<std::endl;
      //cancelThread();
    }
    catch(const CGraphics::Exception &ex)
    {
      std::cerr<<"DebugReciever::doTask(): CGraphics exception: "
               <<ex.what()<<std::endl;
      //cancelThread();
    }
    catch(...)
    {
      // exceptions are NOT allowed...
      std::cerr<<"DebugReciever::doTask(): exception cought..."
               <<std::endl;
      cancelThread();
    };
  };

  struct raster *convert(const CGraphics::Raster &r) const;

  Network::TCPServer         _server;
  DebugWin::RasterSwapper   &_swapper;
}; // class DebugReciever

}; // namespace DebugWin

#endif


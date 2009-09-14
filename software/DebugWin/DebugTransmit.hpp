/*
 * DebugTransmit.hpp
 *
 * debugging transmitter - each time it has new data
 * class tries to connect to given IP:port to send it there.
 *
 * this class should be used whenever debug of graphics
 * must be used!
 *
 */
#ifndef INCLUDE_DEBUGWIN_DEBUGSRV_HPP_FILE
#define INCLUDE_DEBUGWIN_DEBUGSRV_HPP_FILE

#include "Network/TCPClient.hpp"
#include "Graphics/Raster.hpp"
#include "CGraphics/Raster.hpp"
#include "Maps/VectorImg.hpp"


namespace DebugWin
{

class DebugTransmit
{
public:
  // addr - address we will try to connect to
  DebugTransmit(const Network::Address &addr):
    _addr(addr)
  {
  };

  // senders themself:
  void debug(const Graphics::RasterRGB &r );
  void debug(const CGraphics::Raster   &cr); // this one makes internal copy...
  // vi    - image to debug
  // size  - size of output image
  // pos   - position that [0;0] of vi should be placed in (before scalling)
  // k     - scale
  // vFlip - verticaly flip image?
  void debug(const Maps::VectorImg     &vi,
             Graphics::Size             size,
             Graphics::Position         pos,
             double                     k=1,
             bool                       vFlip=false);

private:
  const Network::Address _addr;
}; // class DebugSrv

}; // namespace DebugWin

#endif


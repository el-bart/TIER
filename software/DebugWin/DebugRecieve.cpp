/*
 * DebugRecieve.cpp
 *
 */

#include "DebugRecieve.hpp"

using namespace std;
using namespace CGraphics;


namespace DebugWin
{

struct raster *DebugRecieve::convert(const Raster &r) const
{
  // build-and-wrap C-raster struct pointer
  CRasterHolder crh( raster_create( r.getSize()._x, r.getSize()._y ) );
  if(crh.get()==NULL)
    return NULL;
  struct raster *rh=crh.get();  // helper

  for(unsigned int y=0; y<r.getSize()._y; ++y)
    for(unsigned int x=0; x<r.getSize()._x; ++x)
    {
      // get pixel from input image
      CGraphics::Position pos(x,y);
      CGraphics::Pixel    pix=r.getPixel(pos);

      // assign to raster
      raster_pix_set_r(rh, x,y, pix.getR() );
      raster_pix_set_g(rh, x,y, pix.getG() );
      raster_pix_set_b(rh, x,y, pix.getB() );
    };

  return crh.release();
};

};


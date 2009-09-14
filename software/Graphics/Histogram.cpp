/*
 * Histogram.cpp
 *
 */

#include "Graphics/Histogram.hpp"
#include "Graphics/Size.hpp"
#include "Graphics/Pixel.hpp"
#include "Graphics/Position.hpp"


namespace Graphics
{


Histogram::Histogram(const RasterRGB &raster)
{
  const Size &size=raster.getSize();
  for(unsigned int y=0; y<size.y(); ++y)
    for(unsigned int x=0; x<size.x(); ++x)
    {
      const Position  pos(x,y);
      const PixelRGB &pix=raster.getPixel(pos);
      ++_r[pix._r];
      ++_g[pix._g];
      ++_b[pix._b];
    };
};


// unnamed namespace for smoothing histogram helper
namespace
{

inline void smoothChannel(unsigned int *out, const unsigned int *in)
{
  // first two need to be handled seraretly
  out[0]=(in[0]+in[1]+in[2]      )/3;
  out[1]=(in[0]+in[1]+in[2]+in[3])/4;

  // main processing is schematic
  for(int i=0+2; i<=255-2; ++i)
    out[i]=(in[i-2]+in[i-1]+in[i]+in[i+1]+in[i+2])/5;

  // last two need to be handled separetly
  out[254]=(in[252]+in[253]+in[254]+in[255])/4;
  out[255]=(        in[253]+in[254]+in[255])/3;
};

}; // unnamed namespace


void Histogram::smooth(void)
{
  const Histogram hTmp(*this);    // temporary copy for processing only

  smoothChannel(_r, hTmp._r);
  smoothChannel(_g, hTmp._g);
  smoothChannel(_b, hTmp._b);
};


void Histogram::integrate(void)
{
  for(int i=1; i<256; ++i)
  {
    _r[i]+=_r[i-1];
    _g[i]+=_g[i-1];
    _b[i]+=_b[i-1];
  };
};


}; // namespace Graphics


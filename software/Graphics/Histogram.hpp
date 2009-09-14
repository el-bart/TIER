/*
 * Histogram.hpp
 *
 */
#ifndef INCLUDE_GRAPHICS_HISTOGRAM_HPP_FILE
#define INCLUDE_GRAPHICS_HISTOGRAM_HPP_FILE

#include "Graphics/Pixel.hpp"
#include "Graphics/Raster.hpp"
#include "Graphics/Exception.hpp"


namespace Graphics
{

//
// class representing histogram of the RGB image
//
class Histogram
{
public:
  Histogram(const Graphics::RasterRGB &raster);

  inline unsigned int getR(unsigned char poz) const { return _r[poz]; };
  inline unsigned int getG(unsigned char poz) const { return _g[poz]; };
  inline unsigned int getB(unsigned char poz) const { return _b[poz]; };

  void smooth(void);
  void integrate(void);

private:
  Histogram();      // do not allow undefined instance

  unsigned int _r[256];  // histogram for R
  unsigned int _g[256];  // histogram for G
  unsigned int _b[256];  // histogram for B
};

}; // namespace Graphics

#endif


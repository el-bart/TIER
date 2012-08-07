/*
 * Pixel.hpp
 *
 */
#ifndef INCLUDE_CGRAPHICS_PIXEL_HPP_FILE
#define INCLUDE_CGRAPHICS_PIXEL_HPP_FILE

#include "CGraphics/Exception.hpp"


namespace CGraphics
{

//
// class representing single RGB pixel
//
class Pixel
{
public:
  typedef unsigned char color;      // we use it as a single color

  inline Pixel()
  {
    // after this call pixel has random color
  };
  inline Pixel(color r, color g, color b):
            _r(r),
            _g(g),
            _b(b)
  {
  };

  inline color getR(void) const { return _r; };
  inline color getG(void) const { return _g; };
  inline color getB(void) const { return _b; };

  inline void setRGB(color r, color g, color b)
  {
    _r=r;
    _g=g;
    _b=b;
  };
  inline void setR(color c) { _r=c; };
  inline void setG(color c) { _g=c; };
  inline void setB(color c) { _b=c; };

private:
  color _r;
  color _g;
  color _b;
}; // class Raster


}; // namespace CGraphics


#endif


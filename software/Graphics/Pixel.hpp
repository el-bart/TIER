/*
 * Pixel.hpp
 *
 */
#ifndef INCLUDE_GRAPHICS_PIXEL_HPP_FILE
#define INCLUDE_GRAPHICS_PIXEL_HPP_FILE

#include "Graphics/Exception.hpp"


namespace Graphics
{

// we use it as a single color
typedef unsigned char color;



//
// class representing single grayscale pixel
//
class PixelGray
{
public:
  inline PixelGray()
  {
    // after this call pixel has random color
  };
  inline PixelGray(color c):
                _c(c)
  {
  };

  color _c;
}; // class PixelGray



//
// class representing single RGB pixel
//
class PixelRGB
{
public:

  inline PixelRGB()
  {
    // after this call pixel has random color
  };
  inline PixelRGB(color r, color g, color b):
            _r(r),
            _g(g),
            _b(b)
  {
  };

  color _r;
  color _g;
  color _b;
}; // class PixelRGB


}; // namespace Graphics


#endif


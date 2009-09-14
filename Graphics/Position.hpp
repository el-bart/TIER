/*
 * Position.hpp
 *
 */
#ifndef INCLUDE_GRAPHICS_POSITION_HPP_FILE
#define INCLUDE_GRAPHICS_POSITION_HPP_FILE

#include "Graphics/Exception.hpp"


namespace Graphics
{


//
// class representing position in the image 2D
//
class Position
{
public:
  inline Position(unsigned int x, unsigned int y):
            _x(x),
            _y(y)
  {
  };

  inline void offset(const unsigned int &dx, const unsigned int &dy)
  {
    _x+=dx;
    _y+=dy;
  };

  inline const unsigned int &x(void) const
  {
    return _x;
  };
  inline const unsigned int &y(void) const
  {
    return _y;
  };

  inline void x(const unsigned int &x)
  {
    _x=x;
  };
  inline void y(const unsigned int &y)
  {
    _y=y;
  };

private:
  unsigned int _x;
  unsigned int _y;
};


}; // namespace Graphics


#endif


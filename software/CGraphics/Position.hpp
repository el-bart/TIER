/*
 * Position.hpp
 *
 */
#ifndef INCLUDE_CGRAPHICS_POSITION_HPP_FILE
#define INCLUDE_CGRAPHICS_POSITION_HPP_FILE

#include "CGraphics/Exception.hpp"


namespace CGraphics
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

  const unsigned int _x;
  const unsigned int _y;

private:
  Position();
};


}; // namespace CGraphics


#endif


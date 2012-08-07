/*
 * Size.hpp
 *
 */
#ifndef INCLUDE_CGRAPHICS_SIZE_HPP_FILE
#define INCLUDE_CGRAPHICS_SIZE_HPP_FILE

#include <assert.h>

#include "CGraphics/Exception.hpp"
#include "CGraphics/Position.hpp"


namespace CGraphics
{


//
// class representing size of the image
//
class Size
{
public:
  inline Size(unsigned int x, unsigned int y):
            _x(x),
            _y(y)
  {
    if(x==0 || y==0)
      throw CGraphics::Exception("Size: size must be positive integer");
  };

  // checks whether given position fits in the image of a given size
  inline bool inRange(const CGraphics::Position &pos) const
  {
    return pos._x<_x && pos._y<_y;
  };

  // checks if given sizes are equal
  inline bool operator==(const CGraphics::Size &size) const
  {
    return _x==size._x && _y==size._y;
  };
  // checks if given sizes are NOT equal
  inline bool operator!=(const CGraphics::Size &size) const
  {
    return !(*this==size);
  };
  // checks relation between sizes
  inline bool operator<(const CGraphics::Size &size) const
  {
    return _x<size._x && _y<size._y;
  };

  const unsigned int _x;
  const unsigned int _y;

private:
  Size();
};


}; // namespace CGraphics


#endif


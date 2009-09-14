/*
 * Size.hpp
 *
 */
#ifndef INCLUDE_GRAPHICS_SIZE_HPP_FILE
#define INCLUDE_GRAPHICS_SIZE_HPP_FILE

#include <assert.h>

#include "Graphics/Exception.hpp"
#include "Graphics/Position.hpp"


namespace Graphics
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
      throw Graphics::Exception("Size: size must be positive integer");
  };

  // checks whether given position fits in the image of a given size
  inline bool inRange(const Graphics::Position &pos) const
  {
    return pos.x()<_x && pos.y()<_y;
  };

  // checks if given sizes are equal
  inline bool operator==(const Graphics::Size &size) const
  {
    return _x==size._x && _y==size._y;
  };
  // checks if given sizes are NOT equal
  inline bool operator!=(const Graphics::Size &size) const
  {
    return !(*this==size);
  };
  // checks relation between sizes
  inline bool operator<(const Graphics::Size &size) const
  {
    return _x<size._x && _y<size._y;
  };

  inline unsigned int x(void) const
  {
    return _x;
  };
  inline unsigned int y(void) const
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
  Size();

  unsigned int _x;
  unsigned int _y;
};


}; // namespace Graphics


#endif


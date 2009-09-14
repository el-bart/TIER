/*
 * Destination.hpp
 *
 * this represents dirrection where robot should go.
 * since there can be only 3 logical solutions
 * (left, right and forward) they are represented as
 * enum.
 * to make it more flexible, direction can be also
 * returned as a vector poining to aproperiet direction:
 *
 * [ 0; 1] - forward
 * [-1; 0] - left
 * [ 1; 0] - right
 *
 */
#ifndef INCLUDE_PATH_DESTINATION_HPP_FILE
#define INCLUDE_PATH_DESTINATION_HPP_FILE

#include "Vectors/Vector.hpp"


namespace Path
{

class Destination
{
public:
  typedef enum
  {
    FORWARD,
    LEFT,
    RIGHT
  }
  Direction;


  // dir - destination direction on map
  Destination(Direction dir):
    _dir(dir),
    _dst(0,0)
  {
    // set vector - we will use this!
    switch(dir)
    {
      case FORWARD:
        _dst=Vectors::Vector2DCont( 0, 1);
        break;
      case LEFT:
        _dst=Vectors::Vector2DCont(-1, 0);
        break;
      case RIGHT:
        _dst=Vectors::Vector2DCont( 1, 0);
        break;
    };
  };

  inline const Vectors::Vector2DCont &getDstVect(void) const
  {
    return _dst;
  };

  inline Direction getDstDir(void) const
  {
    return _dir;
  };

private:
  Direction             _dir;
  Vectors::Vector2DCont _dst;
}; // class Destination

}; // namespace Path

#endif


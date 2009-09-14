/*
 * Map2D.hpp
 *
 * class describing 2D map used by TIER for terrain
 * representation and opperation.
 *
 */
#ifndef INCLUDE_MAPS_MAP2D_HPP_FILE
#define INCLUDE_MAPS_MAP2D_HPP_FILE

#include <memory>

#include "Vectors/Space.hpp"
#include "Vectors/RefSpace.hpp"
#include "Maps/Exception.hpp"
#include "Maps/VectorImg.hpp"


namespace Maps
{

// representation of each object in our space
class SpaceObj
{
public:
  typedef Vectors::Line2DCont Line_t;

  SpaceObj(const Line_t &line, unsigned char ttl=2):
    _line(line),
    _ttl(ttl)
  {
  };

  // line-getters:
  inline const Line_t &getLine(void) const
  {
    return _line;
  };
  inline Line_t &getLine(void)
  {
    return _line;
  };

  inline unsigned char getTTL(void) const
  {
    return _ttl;
  };

  // decrease TTL value
  inline void decTTL(void)
  {
    if(0<_ttl)              // if TTL is already zero, do not touch it
      --_ttl;
  };

private:
  Line_t        _line;      // simple 2D line
  unsigned char _ttl;       // TimeToLive for this line
};


// our 2D-Map representation of a SpaceObject
class Map2D
{
public:
  typedef std::auto_ptr<Maps::Map2D>     AutoMap2D;
  typedef Vectors::Space<Maps::SpaceObj> Space_t;

  Map2D(void)
  {
  };
  Map2D(const Maps::VectorImg &vImg);

  // updates current map by informations from input 'map'
  void merge(const Map2D &map);

  // transforms Map2D to new coordinates space
  void transform(const Vectors::RefSpace2D &refSpace);

  inline const Space_t &getSpace(void) const
  {
    return _space;
  };

  // decreases TTL's of all object and removes those with TTL==0
  void updateTTL(void);

private:
  Space_t _space;         // our space in given coordinates
}; // class Map2D

}; // namespace Maps

#endif


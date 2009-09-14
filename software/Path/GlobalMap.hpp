/*
 * GlobalMap.hpp
 *
 * global map for TIER robot - beside the map it includes
 * robots poistion (point) and orientation (vector).
 *
 */
#ifndef INCLUDE_PATH_GLOBALMAP_HPP_FILE
#define INCLUDE_PATH_GLOBALMAP_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Maps/Map2D.hpp"
#include "Path/Exception.hpp"


namespace Path
{

// class is not copyalbe because of the performance...
class GlobalMap: private boost::noncopyable
{
public:
  // baisc initialisation of data - some assumptiona are made...
  GlobalMap(void):
    _pos(0,0),
    _dir(0,1)
  {
  };

  // map - init map with new vaules
  void reset(const Maps::Map2D           &map,
             const Vectors::Point2DCont  &pos,
             const Vectors::Vector2DCont &dir)
  {
    _map=map;
    _pos=pos;
    _dir=dir;
    _dir.normalize();
  };

  // actualize global map, robot's position and orientation
  // by new map (local map given as parameter is in base
  // where robot is in point [0;0], directed in [0;1], which
  // is the default).
  void actualize(const Maps::Map2D &lMap);

  // getters:
  inline const Vectors::Point2DCont &getPos(void) const
  {
    return _pos;
  };
  inline Vectors::Point2DCont getPos(void)
  {
    return _pos;
  };
  inline const Vectors::Vector2DCont &getDir(void) const
  {
    return _dir;
  };
  inline Vectors::Vector2DCont getDir(void)
  {
    return _dir;
  };

  inline const Maps::Map2D &getMap(void) const
  {
    return _map;
  };

  // returns angle (in radians) between direction vector
  // and OY axis (clock-wise).
  inline double getDirOYangle(void) const
  {
    return getDir().getOYangle();
  };

private:
  Maps::Map2D           _map;   // global map of environment
  Vectors::Point2DCont  _pos;   // robot's position
  Vectors::Vector2DCont _dir;   // robot's direction of looking
                                // this vector mus by always
                                // normalised!!!
}; // class GlobalMap

}; // namespace Path

#endif


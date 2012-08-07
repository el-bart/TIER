/*
 * Line.hpp
 *
 * generic class representing single line beetween two
 * points. point type is given as the template argument.
 *
 * note: Point class MUST:
 *        -> beahave as a value
 *        -> implement / double getDist(const Point) const / method
 *
 */
#ifndef INCLUDE_VECTORS_LINE_HPP_FILE
#define INCLUDE_VECTORS_LINE_HPP_FILE

#include "Vectors/Point.hpp"
#include "Vectors/Vector.hpp"
#include "Vectors/Exception.hpp"


namespace Vectors
{

template<typename TPoint>
class Line
{
public:
  // make Line starting in "from" point in direction and
  // length of "dir" Vector.
  Line(const TPoint &from, const Vectors::Vector<TPoint> &dir):
    _from(from),
    _to  (from)
  {
    assert( from.size()==dir.size() );
    unsigned int dims=dir.size();
    for(unsigned int i=0; i<dims; ++i)
      _to[i]+=dir[i];
  };

  Line(const TPoint &from, const TPoint &to):
    _from(from),
    _to  (to  )
  {
  };

  inline const TPoint &getFrom(void) const
  {
    return _from;
  };

  inline const TPoint &getTo(void) const
  {
    return _to;
  };

  inline double getLength(void) const
  {
    return _from.dist(_to);
  };

  inline bool operator==(const Vectors::Line<TPoint> &l) const
  {
    if( _from==l._from && _to==l._to ||
        _from==l._to   && _to==l._from  )
      return true;
    return false;
  };

private:
  TPoint _from;
  TPoint _to;
}; // class Line


//
// some commonly used types of lines
//

typedef Vectors::Line< Vectors::Point2DCont > Line2DCont;
typedef Vectors::Line< Vectors::Point2DDisc > Line2DDisc;


}; // namespace Vectors

#endif


/*
 * RefSpace.hpp
 *
 * referention for space - describes position and
 * orientation of one base (in algebra meaning) regarding
 * to another.
 * can be used for recalculations when moving from one space
 * to another.
 *
 */
#ifndef INCLUDE_VECTORS_REFSPACE_HPP_FILE
#define INCLUDE_VECTORS_REFSPACE_HPP_FILE

#include <math.h>

#include "Vectors/Point.hpp"
#include "Vectors/Vector.hpp"


namespace Vectors
{


template <typename TPoint>
class RefSpace
{
public:
  typedef Vectors::Vector<TPoint> Vector_t;


  //
  // position - position of space from referention space,
  //            given as Vector
  // alfa     - angle beetween OY of destination space and
  //            OY' (OY of our source space) mesured in
  //            radians, clock-wise.
  //
  RefSpace(const Vector_t &position, const double &alfa):
    _pos(position),
    _alfa(alfa)
  {
    // calculation of _alfa%PI :
    const double pi2=2*M_PI;                // 2*PI
    const double da=(0<=_alfa)?-pi2:pi2;    // offset
    while( pi2<=fabs(_alfa) )               // alfa e (-2*PI;+2*PI) ?
      _alfa+=da;                            // 'modulate' alfa!

    // we will use this because of performance
    _sin_a=sin(_alfa);
    _cos_a=cos(_alfa);
  };

  // moves point from original (source) space to reference
  // (destination) space.
  TPoint transform(const TPoint &pt) const;

protected:
  Vector_t _pos;    // offset vector
  double   _alfa;   // rotation angle
  double   _sin_a;  // sin(_alfa)
  double   _cos_a;  // cos(_alfa)
};


//
// common use cases
//

class RefSpace2D: public RefSpace<Vectors::Point2DCont>
{
public:
  typedef Vectors::Line2DCont Line_t;

  RefSpace2D(const Vector_t &position, const double &alfa):
    RefSpace<Vectors::Point2DCont>(position, alfa)
  {
  };

  inline Vectors::Point2DCont transform(const Vectors::Point2DCont &pt) const
  {
    // transformaion is:
    //   Ybase=[sin_a; cos_a]
    //   Xbase=[cos_a;-sin_a]
    const double x= pt[0]*_cos_a + pt[1]*_sin_a;
    const double y=-pt[0]*_sin_a + pt[1]*_cos_a;

    Vectors::Point2DCont pOut( x+_pos[0], y+_pos[1] ); // verctor + offset
    return pOut;                                       // return result
  };

  inline Line_t transform(const Line_t &ln) const
  {
    Vectors::Point2DCont pf=transform( ln.getFrom() );  // tansform point
    Vectors::Point2DCont pt=transform( ln.getTo()   );  // ...
    Line_t lOut(pf, pt);                                // make dest. line
    return lOut;                                        // return result
  };
};

}; // namespace Vectors

#endif


/*
 * VectorImg.cpp
 *
 */
#include <algorithm>

#include "VectorImg.hpp"

using namespace Maps;
using namespace Vectors;


namespace Maps
{

// unnamed namespace used for StrictWeakOrdering
// implementation for sorting purposes.
namespace
{

struct Line2DSWO
{
inline bool operator()(const VectorImg::Line &l1,
                       const VectorImg::Line &l2) const
{
  // we need points to be in left-to-right order
  Point2DCont p1_f;
  Point2DCont p1_t;
  if( pointCmp<0,1>( l1.getFrom(), l1.getTo() ) )
  {
    p1_f=l1.getFrom();
    p1_t=l1.getTo();
  }
  else
  {
    p1_f=l1.getTo();
    p1_t=l1.getFrom();
  };

  // we need points to be in left-to-right order
  Point2DCont p2_f;
  Point2DCont p2_t;
  if( pointCmp<0,1>( l2.getFrom(), l2.getTo() ) )
  {
    p2_f=l2.getFrom();
    p2_t=l2.getTo();
  }
  else
  {
    p2_f=l2.getTo();
    p2_t=l2.getFrom();
  };


  // l1 < l2  ?
  bool ret;
  if( pointCmp<1,0>(p1_f, p2_f) )
    ret=true;
  else
    if( p1_f==p2_f && pointCmp<1,0>(p1_t, p2_t) )
      ret=true;
    else
      ret=false;

  return ret;
};

private:
// SWO for two points
template <int ind1, int ind2>
inline bool pointCmp(const Point2DCont &p1, const Point2DCont &p2) const
{
  if(p1[ind1]<p2[ind1])
    return true;
  else
    if(p1[ind1]==p2[ind1] && p1[ind2]<p2[ind2])
      return true;
  return false;
};

}; // struct Line2DSWO

}; // unnamed namespace


VectorImg::VectorImg(unsigned int maxLinesCnt)
{
  _elems.reserve(maxLinesCnt);
};


VectorImg::VectorImg(const Space<Line> &space)
{
  _elems.reserve( space.linesCnt() );   // reserve space for output
  for(Space<Line>::const_iterator it=space.begin();
      it!=space.end(); ++it )           // copy data to vector
    _elems.push_back(*it);              // ...

  assert( space.linesCnt()==_elems.size() );
  Line2DSWO lineSWO;                    // temporary object
  sort( _elems.begin(), _elems.end(), lineSWO );        // sort lines
};


}; // namespace Maps


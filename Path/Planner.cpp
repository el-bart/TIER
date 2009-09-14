/*
 * Planner.cpp
 *
 */
#include <math.h>
#include <algorithm>

#include "Vectors/Vector.hpp"
#include "Planner.hpp"

using namespace std;
using namespace Vectors;
using namespace Maps;


namespace Path
{

Planner::Planner(double margin, double minObstacleTotalLen):
  _margin(margin),
  _minObstacleTotalLen(minObstacleTotalLen)
{
  if(_margin<0.0)
    throw Exception("Planner::Planner(): "
                    "margin must by non-negative");
  if(_minObstacleTotalLen<0.0)
    throw Exception("Planner::Planner(): "
                    "minObstacleTotalLen must by non-negative");
};



namespace
{

// check if a given point is in range
inline bool isInRange(const Point2DCont &p1, const Point2DCont &p2,
                      const Point2DCont &pT, const double &margin)
{
  // this solutions gives better effect, since local decisions are
  // far more suboptimal than global ones, therefore robot needs
  // much more space to navigate freely.
  // this i why we assume that all obstacles are on our colision
  // path with us.
  return true;

  const double min=0.0001;      // everything below this value
                                // will be threated as zero
  // length of triangle's sides
  const double a=Vector2DCont(p1,pT).length();
  const double b=Vector2DCont(pT,p2).length();
  const double c=Vector2DCont(p1,p2).length();

  if(a<margin || b<margin)      // test point (pT) is too close to
    return true;                // either source or destination point?

  // Heron's triangle's field:
  const double p=(a+b+c)/2.0;
  const double P=sqrt( p*(p-a)*(p-b)*(p-c) );

  if(P<min)                     // this is not a tirangle?
    if( fabs( c-(a+b) )<min )   // point is inside?
      return true;
    else                        // point is outsise...
      return false;

  // how far is out point from line?
  const double h=P/(0.5*c);     // this goes from P=1/2*a*h comapred with
                                // Heron's method
  if(margin<h)                  // is point too far away?
    return false;

  if(c<a || c<b)                // point is outside out interest?
    return false;

  return true;                  // so the point IS in range...
}; // isInRange()


// class to be used as a template for countObstaclesTotalLenIf
class OpTotalObstacleLen
{
public:
  OpTotalObstacleLen(const Point2DCont &p1,
                     const Point2DCont &p2,
                     double margin):
    _p1(p1),
    _p2(p2),
    _margin(margin),
    _totalLenLeft  (0),
    _totalLenRight (0),
    _totalLenMiddle(0)
  {
  };

  virtual ~OpTotalObstacleLen(void)
  {
  };

  inline void operator()(const SpaceObj &spaceObj)
  {
    const Line2DCont  &l =spaceObj.getLine();
    const Point2DCont &pf=l.getFrom();
    const Point2DCont &pt=l.getTo();

    // if line colides, count it in!
    if( isInRange(_p1,_p2, pf, _margin) ||
        isInRange(_p1,_p2, pt, _margin)    )
    {
      const double tmp=weight(_p1,l)*l.getLength();    // cnt sum
      // now choose where to count line to:
      if( pf[0]<0 && pt[0]<0 )      // on left?
        _totalLenLeft+=tmp;
      else
        if( 0<pf[0] && 0<pt[0] )    // on right?
          _totalLenRight+=tmp;
        else                        // the "middle" case!
          _totalLenMiddle+=tmp;
    };
  };

  // by default we don't use weight ;)
  virtual double weight(const Point2DCont &pStart,
                        const Line2DCont  &l       )
  {
    return 1;
  };

  inline double getTotalLen(void) const
  {
    return getTotalLenLeft()+getTotalLenRight()+getTotalLenMiddle();
  };

  inline double getTotalLenLeft(void) const
  {
    return _totalLenLeft;
  };

  inline double getTotalLenRight(void) const
  {
    return _totalLenRight;
  };

  inline double getTotalLenMiddle(void) const
  {
    return _totalLenMiddle;
  };

private:
  const Point2DCont &_p1;
  const Point2DCont &_p2;
  const double       _margin;
  double             _totalLenLeft;
  double             _totalLenRight;
  double             _totalLenMiddle;
}; // class OpTotalObstacleLen


// this class basicly does the same as OpTotalObstacleLen
// but this time we use weights! :)
class OpCountWeightenLengths: public OpTotalObstacleLen
{
public:
  OpCountWeightenLengths(const Point2DCont &p1,
                         const Point2DCont &p2,
                         double margin):
    OpTotalObstacleLen(p1,p2, margin)
  {
  };

  // cout weights as reverse of sum of distances between robot
  // and two points of line
  virtual double weight(const Point2DCont &pStart,
                        const Line2DCont  &l       )
  {
    const double w=pStart.dist( l.getFrom() ) +
                   pStart.dist( l.getTo()   );
    return 1/(1+w);
  };
}; // class OpCountWeightenLengths


// performs computation of sum by a given template on given gMap
template <typename TOperation>
inline TOperation operateOnLines(const GlobalMap &gMap, const double &margin)
{
  Vector2DCont      vd=gMap.getDir();
  vd*=2*margin;             // how "far" we need to have clean space?
  const Point2DCont p1(    0,     0);
  const Point2DCont p2(vd[0], vd[1]);

  const Map2D::Space_t &space=gMap.getMap().getSpace(); // get space to check

  // check if anything is within +-marign in front of us:
  return for_each<Map2D::Space_t::const_iterator, TOperation>
            ( space.begin(), space.end(), TOperation(p1,p2, margin) );
}; // operateOnLines


}; // unnamed namespace


Plan Planner::findPlan(const GlobalMap &gMap, const Plan &pPlan)
{
  Destination dst(Destination::FORWARD);

  if( isTrackClean(gMap) )          // if there is no obstcle on our way
    return Plan(dst);               // we can safely move forward :)

  //
  // if we're here, it means there IS obstacle on our way
  //

  //
  // check, whether we've already made a decision - if
  // we're already turning somewhere, better live it that
  // way to avoid stupid ping-pongs from left to right...
  //
  if( pPlan.getDst().getDstDir()!=Destination::FORWARD )
    return pPlan;

  //
  // compute total obstacle's lengths on left and
  // right site of robot
  //
  const OpCountWeightenLengths ocwl=
            operateOnLines<OpCountWeightenLengths>(gMap, _margin);

  //
  // now we need to choose direction to take a trun. heuristic is
  // used here, saying that there is smaller chance of sighting more
  // obstacles when turn is beeing made on direction oposite to
  // the one where more can be seen now (purely local optimalization).
  //
  if( ocwl.getTotalLenLeft() < ocwl.getTotalLenRight() )
    dst=Destination(Destination::LEFT );
  else
    dst=Destination(Destination::RIGHT);

  // return plan with a given direction
  return Plan(dst);
};


bool Planner::isTrackClean(const GlobalMap &gMap) const
{
  // check if anything is within +-marign in front of us:
  const OpTotalObstacleLen otol=
            operateOnLines<OpTotalObstacleLen>(gMap, _margin);
  return otol.getTotalLen()<_minObstacleTotalLen;
};

}; // namespace Path


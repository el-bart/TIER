/*
 * Planner.hpp
 *
 * path planner main class - finds plan for robot.
 *
 */
#ifndef INCLUDE_PATH_PLANNER_HPP_FILE
#define INCLUDE_PATH_PLANNER_HPP_FILE

#include "Path/Plan.hpp"
#include "Path/GlobalMap.hpp"
#include "Path/Exception.hpp"


namespace Path
{

class Planner
{
public:
  // margin -              radius from a robot's current position, given
  //                       as a point, that must be clean for a robot to
  //                       move/rotate freely (in [m]).
  // minObstacleTotalLen - minimul total length of "colliding" obstacles
  //                       to be threated as real obstacle (in [m]).
  Planner(double margin, double minObstacleTotalLen);

  // returns plan for robot given global map 'gMap' and
  // previous plan.
  Path::Plan findPlan(const Path::GlobalMap &gMap,
                      const Path::Plan      &pPlan);

private:
  // indicates wheteher route from p1 to p2 clean (colisionles)
  bool isTrackClean(const Path::GlobalMap &gMap) const;

  const double _margin;
  const double _minObstacleTotalLen;
}; // class Planner

}; // namespace Path

#endif


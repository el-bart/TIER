/*
 * PathCreator.hpp
 *
 * class wrapping whole plan creation and actualization
 * process. gives back reference to current plan.
 *
 */
#ifndef INCLUDE_ZUCHTER_PATHCREATOR_HPP_FILE
#define INCLUDE_ZUCHTER_PATHCREATOR_HPP_FILE

#include <string>
#include <memory>

#include "Path/Planner.hpp"
#include "Path/GlobalMap.hpp"


namespace Zuchter
{

// listener of events for PathCreator
struct PathCreatorListener
{
  virtual ~PathCreatorListener(void)
  {
  };

  // called on error
  virtual void onError(const std::string &why) = 0;
};


class PathCreatorParameters
{
public:
  // margin              - how far from center of robot must be clean
  //                       to allow robot to move/rotate freely (in [m]).
  // minObstacleTotalLen - minimum total length of obstacles in collision
  //                       range to be threated as non-clear path.
  PathCreatorParameters(Zuchter::PathCreatorListener    &listener,
                        const double                     margin,
                        const double                     minObstacleTotalLen):
    _listener(listener),
    _margin(margin),
    _minObstacleTotalLen(minObstacleTotalLen)
  {
  };

  Zuchter::PathCreatorListener    &_listener;
  const double                     _margin;
  const double                     _minObstacleTotalLen;
}; // class PathCreatorParameters


class PathCreator
{
public:
  PathCreator(Zuchter::PathCreatorParameters params);

  // creates/updates plan and returns reference to it.
  const Path::Plan &createPath(const Path::GlobalMap &gMap);

private:
  Zuchter::PathCreatorParameters _params;
  Path::Planner                  _planner;
  Path::Plan                     _plan;
}; // class PathCreator

}; // namespace Zuchter

#endif


/*
 * PathCreator.cpp
 *
 */

#include "PathCreator.hpp"

using namespace std;
using namespace Path;
using namespace Vectors;


namespace Zuchter
{


PathCreator::PathCreator(PathCreatorParameters params):
  _params(params),
  _planner(_params._margin, _params._minObstacleTotalLen)
{
};


const Plan &PathCreator::createPath(const GlobalMap &gMap)
{
  try
  {
    // choose where to go now:
    _plan=_planner.findPlan(gMap, _plan);
  }
  catch(const Path::Exception &e)
  {
    string err=string("PathCreator::createPath(): ") + e.what();
    _params._listener.onError(err);
  }
  catch(...)
  {
    string err=string("PathCreator::createPath(): unknown exception cought");
    _params._listener.onError(err);
  };

  return _plan;
};


}; // namespace Zuchter


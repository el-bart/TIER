/*
 * TIERctl.cpp
 *
 */

#include "TIERctl.hpp"

using namespace std;
using namespace Path;


namespace Zuchter
{


TIERctl::TIERctl(const MapCreatorParameters  &mapCreatorParams,
                 PathCreatorParameters        pathCreatorParams,
                 CommandCreatorParameters    &commandCreatorParams,
                 time_t                       startDelay):
  _mapCreator(mapCreatorParams),
  _pathCreator(pathCreatorParams),
  _commandCreator(commandCreatorParams),
  _startTime( time(NULL) ),
  _startDelay(startDelay)
{
  _commandCreator.start();        // run communication thread
};


void TIERctl::process(void)
{
  const GlobalMap &gMap=_mapCreator.updateGlobalMap();  // get actual map
  const Plan      &plan=_pathCreator.createPath(gMap);  // route path through
  if( _startTime+_startDelay < time(NULL) )
    _commandCreator.update(plan, gMap);                 // create & send new
                                                        // commands to robot
};


void TIERctl::stopTIER(void)
{
  _commandCreator.stopTIER();
};


bool TIERctl::reconnect(void)
{
  return _mapCreator.reconnect();
};

}; // namespace Zuchter


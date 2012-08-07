/*
 * CommandCreator.cpp
 *
 */
#include <math.h>
#include <unistd.h>

#include "CommandCreator.hpp"


using namespace Path;
using namespace Vectors;
using namespace Threads;
using namespace CommProto;


namespace Zuchter
{

CommandCreator::CommandCreator(const CommandCreatorParameters &params):
  _params(params),
  _spdSet(0, 0)         // stop by default!
{
  if( !_params._proto.init() )
    _params._listener.onCommError("CommandCreator::CommandCreator(): "
                                  "proto.init() failed");
};

CommandCreator::~CommandCreator(void)
{
  stop();               // end communication thread

  // send multiple stops, just in case...
  for(int i=0; i<3; ++i)
    _params._proto.stop();
};

void CommandCreator::update(const Plan &plan, const GlobalMap &gMap)
{
  // make vector from current position to destination
  const Vector2DCont &vp=plan.getDst().getDstVect();
  // get vector of direction of robot
  const Vector2DCont &vd=gMap.getDir();

  // get angle between robot's direcion vector and destination in
  // radians (clock-wise)
  double angle=vd.getAngle(vp);

  TIERproto::Speed spd(0,0); // speed to set
  if( angle<_params._tolerance ||
      2*M_PI-_params._tolerance<angle)      // misdirection is suficiently
    spd=TIERproto::Speed( 250,  250);       // small not to change dir?
  else
    if(angle<M_PI)           // turn right?
      spd=TIERproto::Speed( 250, -250);
      //spd=TIERproto::Speed(   0,-250);
    else                     // turn left?
      spd=TIERproto::Speed(-250,  250);
      //spd=TIERproto::Speed(-250,   0);

  // critical section - set new speed
  {
    Lock lock(_mutex);
    _spdSet=spd;
  };
};


void CommandCreator::stopTIER(void)
{
  // do it in critical section
  Lock lock(_mutex);
  _spdSet=TIERproto::Speed(0, 0);
};


void CommandCreator::doTask(void)
{
  TIERproto::Speed spd(0,0);          // this copy is needed because of very
                                      // high command sending times...
  // critical section - make copy of speed setting
  {
    Lock lock(_mutex);
    spd=_spdSet;
  };

  // apply settings
  if( !_params._proto.spdst(spd) )    // set new speed to robot
    _params._listener.onCommError("CommandCreator::doTask(): "
                                  "proto.spdst() failed");
  usleep(200*1000);                   // 200[ms] - this is needed since radio
                                      // modems seems not to handle more...
};

}; // namespace Zuchter


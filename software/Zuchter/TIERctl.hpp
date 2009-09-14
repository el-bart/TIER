/*
 * TIERctl.hpp
 *
 * main class holding all elements required to control
 * TIER's behaviour.
 *
 */
#ifndef INCLUDE_ZUCHTER_TIERCTL_HPP_FILE
#define INCLUDE_ZUCHTER_TIERCTL_HPP_FILE

#include <time.h>
#include <boost/noncopyable.hpp>

#include "Zuchter/MapCreator.hpp"
#include "Zuchter/PathCreator.hpp"
#include "Zuchter/CommandCreator.hpp"


namespace Zuchter
{

class TIERctl: private boost::noncopyable
{
public:
  TIERctl(const Zuchter::MapCreatorParameters &mapCreatorParams,
          Zuchter::PathCreatorParameters       pathCreatorParams,
          Zuchter::CommandCreatorParameters   &commandCreatorParams,
          time_t                               startDelay=2);

  // main execution point
  void process(void);

  //
  // other commands, that are usable for the outside world :)
  //

  // send stop request to robot
  void stopTIER(void);

  // reconnect to V4L server and return true if succeded
  bool reconnect(void);

private:
  Zuchter::MapCreator     _mapCreator;
  Zuchter::PathCreator    _pathCreator;
  Zuchter::CommandCreator _commandCreator;
  time_t                  _startTime;       // when it all started... ;)
  time_t                  _startDelay;      // this is init-time, used for
                                            // delaying a little executing
                                            // of commands to allow camera
                                            // picture to stabilize. (in [s])
}; // class TIERctl

}; // namespace Zuchter

#endif


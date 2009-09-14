/*
 * CommandCreator.hpp
 *
 * class interpreting plan, converting it to commands
 * (current) and sending them to robot.
 *
 */
#ifndef INCLUDE_ZUCHTER_COMMANDCREATOR_HPP_FILE
#define INCLUDE_ZUCHTER_COMMANDCREATOR_HPP_FILE

#include <string>
#include <assert.h>

#include "CommProto/TIERproto.hpp"
#include "Path/GlobalMap.hpp"
#include "Path/Plan.hpp"
#include "Threads/Thread.hpp"
#include "Threads/Lock.hpp"
#include "Threads/Mutex.hpp"


namespace Zuchter
{

struct CommandCreatorListener
{
  virtual ~CommandCreatorListener(void)
  {
  };

  // called when communication error ocurres
  virtual void onCommError(const std::string &why) = 0;
  // called when generic error spoted
  virtual void onError(const std::string &why) = 0;
}; // struct CommandCreatorListener


struct CommandCreatorParameters
{
  // listener  - listener for events genereted by class
  // proto     - protocol to use to communiacate with robot
  // tolerance - tolerance of missdireciont between robot's orientation
  //             and vector to destination point, to be threated as
  //             "matching", measured in radians
  CommandCreatorParameters(Zuchter::CommandCreatorListener &listener,
                           CommProto::TIERproto            &proto,
                           const double                    &tolerance):
    _listener(listener),
    _proto(proto),
    _tolerance(tolerance)
  {
    assert(0<=_tolerance);
  };

  Zuchter::CommandCreatorListener &_listener;
  CommProto::TIERproto            &_proto;
  const double                     _tolerance;
}; // struct CommandCreatorParameters




class CommandCreator: public Threads::Thread
{
public:
  CommandCreator(const Zuchter::CommandCreatorParameters &params);
  virtual ~CommandCreator(void);

  // executes new commands accordingly to current plan and situation
  // (represented by 'plan' and 'gMap' respectively).
  void update(const Path::Plan &plan, const Path::GlobalMap &gMap);

  // sends stop request to robot - this is useful in exception
  // situations, when it is important to stop robot before
  // closing application.
  void stopTIER(void);

private:
  virtual void doTask(void);        // thread implementation

  Zuchter::CommandCreatorParameters _params;
  CommProto::TIERproto::Speed       _spdSet;   // command that will be send
  Threads::Mutex                    _mutex;
}; // class Command Creator

}; // namespace Zuchter

#endif


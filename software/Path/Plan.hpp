/*
 * Plan.hpp
 *
 * representation of plan for robot's movement.
 *
 */
#ifndef INCLUDE_PATH_PLAN_HPP_FILE
#define INCLUDE_PATH_PLAN_HPP_FILE

#include "Path/Destination.hpp"
#include "Path/Exception.hpp"


namespace Path
{

class Plan
{
public:
  // dst - destination to go to (by default - forward)
  Plan(Destination dst=Destination::FORWARD):
    _dst(dst)
  {
  };

  // change destination
  inline void changeDst(Destination dst)
  {
    _dst=dst;
  };

  // returns Destination/Current posiotion of robot
  inline Destination getDst(void) const
  {
    return _dst;
  };

private:
  Destination _dst;     // destination dirrection
}; // namespace Plan

}; // namespace Path

#endif


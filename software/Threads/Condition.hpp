/*
 * Condition.hpp
 *
 * class wrapping around conditional variable from
 * pthreads.
 *
 */
#ifndef INCLUDE_THREADS_CONDITION_HPP_FILE
#define INCLUDE_THREADS_CONDITION_HPP_FILE

#include <pthread.h>
#include <boost/noncopyable.hpp>

#include "Threads/Exception.hpp"


namespace Threads
{

// forward declaration
class Threads::Conditional;


class Condition: private boost::noncopyable
{
public:
  Condition(void)
  {
    if( pthread_cond_init(&_cond, NULL)!=0 )
      throw Threads::Exception("Condition::Condition(): "
                               "pthread_cond_init() failed");
  };
  ~Condition(void)
  {
    pthread_cond_destroy(&_cond);
  };

private:
  friend class Threads::Conditional;

  pthread_cond_t _cond;     // conditional variable
}; // class Condition

}; // namespace Threads

#endif


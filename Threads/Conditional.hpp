/*
 * Conditional.hpp
 *
 * class implementing functionality of pthread's
 * conditional variable.
 *
 */
#ifndef INCLUDE_THREADS_CONDITIONAL_HPP_FILE
#define INCLUDE_THREADS_CONDITIONAL_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Threads/Mutex.hpp"
#include "Threads/Condition.hpp"
#include "Threads/Exception.hpp"


namespace Threads
{

class Conditional: private boost::noncopyable
{
public:
  // test - determines if condition is met or not
  Conditional(bool test=false):
    _test(test)
  {
  };

  // wait for condition to be met
  inline void wait(void)
  {
    Lock lock(_mutex);
    // wait and check if condition is met:
    while( !testCondition() )
      if( pthread_cond_wait(&_cond._cond, &_mutex._mutex)!=0 )
        throw Threads::Exception("Conditional::wait(): "
                                 "pthread_cond_wait() failed");
  };

  // signal one thread
  inline void signalOne(void)
  {
    Lock lock(_mutex);
    if( pthread_cond_signal(&_cond._cond)!=0 )
      throw Threads::Exception("Conditional::signalOne(): "
                               "pthread_cond_signal() failed");
  };

  // signal all of threads
  inline void signalAll(void)
  {
    Lock lock(_mutex);
    if( pthread_cond_broadcast(&_cond._cond)!=0 )
      throw Threads::Exception("Conditional::signalAll(): "
                               "pthread_cond_broadcast() failed");
  };

  // set value of condition (test) variable
  inline void setCondition(bool val)
  {
    Lock lock(_mutex);
    _test=val;
  };

private:
  inline bool testCondition(void)
  {
    Lock lock(_mutex);
    return _test;
  };

  Threads::Mutex     _mutex;
  Threads::Condition _cond;
  bool               _test;
}; // class Conditional

}; // namespace Threads

#endif


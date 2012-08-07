/*
 * GeneralLock.hpp
 *
 * critical sections basic class.
 *
 */
#ifndef INCLUDE_THREADS_GENERALLOCK_HPP_FILE
#define INCLUDE_THREADS_GENERALLOCK_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Threads/Mutex.hpp"
#include "Threads/Exception.hpp"


namespace Threads
{

class GeneralLock: private boost::noncopyable
{
protected:
  GeneralLock(Threads::Mutex &mutex):
    _mutex(mutex)
  {
  };

  inline void lock(void)
  {
    _mutex.lock();
  };
  inline bool tryLock(void)
  {
    return _mutex.tryLock();
  };
  inline void unlock(void)
  {
    _mutex.unLock();
  };

  Threads::Mutex &_mutex;
}; // class Lock

}; // namespace Threads

#endif


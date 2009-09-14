/*
 * Lock.hpp
 *
 * critical section implementation.
 *
 */
#ifndef INCLUDE_THREADS_LOCK_HPP_FILE
#define INCLUDE_THREADS_LOCK_HPP_FILE

#include "Threads/Mutex.hpp"
#include "Threads/GeneralLock.hpp"
#include "Threads/Exception.hpp"


namespace Threads
{

class Lock: private Threads::GeneralLock
{
public:
  Lock(Threads::Mutex &mutex):
    GeneralLock(mutex)
  {
    GeneralLock::lock();
  };

  ~Lock(void)
  {
    GeneralLock::unlock();
  };
}; // class Lock

}; // namespace Threads

#endif


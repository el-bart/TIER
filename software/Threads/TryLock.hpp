/*
 * TryLock.hpp
 *
 * critical section implementation. locks mutex only
 * if locking is possible.
 *
 * note: construction DOESN'T lock mutex by default!
 *
 */
#ifndef INCLUDE_THREADS_LOCK_HPP_FILE
#define INCLUDE_THREADS_LOCK_HPP_FILE

#include "Threads/Mutex.hpp"
#include "Threads/GeneralLock.hpp"
#include "Threads/Exception.hpp"


namespace Threads
{

class TryLock: private Threads::GeneralLock
{
public:
  // note - constructor DOESN'T lock mutex!
  TryLock(Threads::Mutex &mutex):
    GeneralLock(mutex),
    _locks(0)
  {
  };

  ~TryLock(void)
  {
    while(_locks)       // unlock if was locked
    {
      GeneralLock::unlock();
      --_locks;
    };
  };

  inline bool tryLock(void)
  {
    unsigned int prevLocks=_locks;
    if( GeneralLock::tryLock() )        // locking succeded?
      ++_locks;
    return _locks==prevLocks;           // number of locks changed?
  };

private:
  unsigned int _locks;  // number of locks on mutex
}; // class Lock

}; // namespace Threads

#endif


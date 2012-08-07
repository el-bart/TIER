/*
 * Mutex.hpp
 *
 * represents mutex. lock/unlock is made by
 * Lock friend class
 *
 */
#ifndef INCLUDE_THREADS_MUTEX_HPP_FILE
#define INCLUDE_THREADS_MUTEX_HPP_FILE

#include <stdlib.h>
#include <pthread.h>
#include <boost/noncopyable.hpp>

#include "Threads/Exception.hpp"


namespace Threads
{

// forward declarations
class GeneralLock;
class Conditional;


class Mutex: private boost::noncopyable
{
public:
  Mutex(void):
    _locks(0)
  {
    pthread_mutexattr_t mta;
    pthread_mutexattr_init(&mta);
    pthread_mutexattr_settype(&mta, PTHREAD_MUTEX_RECURSIVE);

    int ret=pthread_mutex_init(&_mutex, &mta);

    pthread_mutexattr_destroy(&mta);

    if(ret!=0)
      throw Threads::Exception("Mutex::Mutex(): "
                               "pthread_mutex_init() failed()");
  };
  ~Mutex(void)
  {
    unLock();
    pthread_mutex_destroy(&_mutex);
  };

private:

  friend class Threads::GeneralLock;
  friend class Threads::Conditional;

  // everything below can be used by GeneralLock
  inline void lock(void)
  {
    ++_locks;
    pthread_mutex_lock(&_mutex);
  };

  inline bool tryLock(void)
  {
    unsigned int prevLocks=_locks;
    if( pthread_mutex_trylock(&_mutex) ) // try locking...
      ++_locks;
    return _locks==prevLocks;            // number of locks increased?
  };

  inline void unLock(void)
  {
    --_locks;
    pthread_mutex_unlock(&_mutex);
  };

  pthread_mutex_t _mutex;
  unsigned int    _locks;   // numer of 'locks' on mutex
}; // class Mutex

}; // namespace Threads

#endif


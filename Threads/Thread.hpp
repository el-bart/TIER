/*
 * Thread.hpp
 *
 * thread object - specyfies how threads are beeing run.
 * th use it, derive from this class and implement doTask()
 * method.
 *
 */
#ifndef INCLUDE_THREADS_THREAD_HPP_FILE
#define INCLUDE_THREADS_THREAD_HPP_FILE

// this is HIGHLY recommended for the kernel code...
#define _MULTI_THREADED

#include <iostream>
#include <pthread.h>
#include <boost/noncopyable.hpp>

#include "Threads/Exception.hpp"


namespace Threads
{

class Thread: private boost::noncopyable
{
public:
  Thread(void):
    _finish(false),
    _isRunning(false)
  {
  };
  virtual ~Thread(void)
  {
    stop();
  };

  inline void start(void)
  {
    if( isRunning() )
      throw Threads::Exception("Thread::start(): thread's already running");

    _finish=false;

    // set attributes for threads, since default values doesn't work
    // well on some systems...
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setschedpolicy (&attr, SCHED_OTHER            );
    pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED  );
    pthread_attr_setscope       (&attr, PTHREAD_SCOPE_SYSTEM   );

    if( pthread_create(&_thread, &attr, Thread::run, this)!=0 )
      throw Threads::Exception("Thread::start(): thread creation failed");

    _isRunning=true;
  };

  inline void stop(void)
  {
    if( !isRunning() )              // multiple 'stops' allowed
      return;
    cancelThread();
    waitForFinish();
  };

  inline bool isRunning(void) const
  {
    return _isRunning;
  };

protected:
  virtual void doTask(void) = 0;    // user implements this

  inline void cancelThread(void)
  {
    _finish=true;
  };

private:
  static inline void *run(void *ptr)
  {
    try
    {
      Threads::Thread &th=*reinterpret_cast<Threads::Thread*>(ptr);
      while(!th._finish)
        th.doTask();
    }
    catch(...)
    {
      std::cerr<<"Thread::run(): exception cought..."<<std::endl;
      assert(!"Thread::run(): doTask() may NOT throw at any circumstances");
    };
    return NULL;
  };

  inline void waitForFinish(void)
  {
    if( !isRunning() )
      throw Threads::Exception("Thread::waitForFinish(): thread not running");

    pthread_join(_thread, NULL);
    _isRunning=false;
    _finish   =false;
  };

  pthread_t _thread;                // thread 'reference'
  bool      _finish;                // should we finish?
  bool      _isRunning;             // is thread running?
};

}; // namespace Threads

#endif


/*
 * TEST.cpp
 *
 */
#include <iostream>
#include <unistd.h>
#include <assert.h>

#include "Mutex.hpp"
#include "GeneralLock.hpp"
#include "Lock.hpp"
#include "TryLock.hpp"
#include "Thread.hpp"
#include "Conditional.hpp"

using namespace std;
using namespace Threads;


class TestThread: public Thread
{
public:
  TestThread(int *data, int size, Mutex &mutex):
    _data(data),
    _size(size),
    _mutex(mutex)
  {
  };

  virtual void doTask(void)
  {
    int seed=seed+getpid();     // some 'pseudo-seed'

    // critical section
    {
      Lock lock(_mutex);        // comment out this to cause assertaion failure
      // write:
      for(int i=0; i<_size; ++i)
        _data[i]=seed+i;
      // check:
      for(int i=_size-1; i>0; --i)
      {
        assert(_data[i]==seed+i);
        assert(_data[i]==_data[i-1]+1);
      };
    };
  };

private:
  int   *_data;
  int    _size;
  Mutex &_mutex;
};


class TestThreadCond: public Thread
{
public:
  TestThreadCond(Conditional &cond, const string &name):
    _cond(cond),
    _name(name)
  {
  };

  virtual void doTask(void)
  {
    cout<<_name<<": wait()"<<endl;
    _cond.wait();
    cout<<_name<<": continue..."<<endl;
    sleep(3);
  };

private:
  Conditional  &_cond;
  const string  _name;
};


int main(void)
{
  cout<<"TEST set 1:"<<endl<<endl;
  {
    Mutex m;
    int data[1024];
    TestThread th1(data, 1024, m);
    TestThread th2(data, 1024, m);

    cerr<<"starting 1... ";
    assert( !th1.isRunning() );
    th1.start();
    assert(  th1.isRunning() );
    cerr<<"done"<<endl;

    cerr<<"starting 2... ";
    th2.start();
    cerr<<"done"<<endl;

    // wait a while...
    sleep(3);

    cerr<<"stopping 1... ";
    th1.stop();
    cerr<<"done"<<endl;

    cerr<<"stopping 2... ";
    th2.stop();
    cerr<<"done"<<endl;
  }
  cout<<endl<<endl<<"====================="<<endl<<endl<<endl;
  cout<<"TEST set 2:"<<endl<<endl;
  {
    Conditional c;
    TestThreadCond th1(c, "th1");
    TestThreadCond th2(c, "th2");

    cout<<"running"<<endl;
    cout<<"\t1..."<<endl;
    th1.start();
    cout<<"\t2..."<<endl;
    th2.start();

    cout<<"wait..."<<endl;
    sleep(1);
    cout<<"resumming one by one"<<endl;
    c.setCondition(true);
    cout<<"\t1/2"<<endl;
    c.signalOne();
    cout<<"\twait"<<endl;
    sleep(1);
    cout<<"\t2/1"<<endl;
    c.signalOne();
    sleep(1);

    cout<<"block conditional again"<<endl;
    c.setCondition(false);
    cout<<"wait"<<endl;
    sleep(4);
    cout<<"resume all"<<endl;
    c.setCondition(true);
    c.signalAll();
    cout<<"wait"<<endl;
    sleep(1);

    cout<<"stopping"<<endl;
    cout<<"\t1"<<endl;
    th1.stop();
    cout<<"\t2"<<endl;
    th2.stop();

    cout<<"ALL DONE :)"<<endl;
  };

  return 0;
}; // main()


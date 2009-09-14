/*
 * TEST.cpp
 *
 */
#include <iostream>
#include <time.h>

#include "TIERproto.hpp"
#include "ProtoLocal.hpp"

using namespace std;
using namespace CommProto;

void wait(void)
{
  timespec dt={2,0};
  nanosleep(&dt, NULL);
};


int main(const int argc, const char * const * const argv )
{
  try
  {
    ProtoLocal pl(ProtoLocal::LPT0);

    if( !pl.init() )
      cerr<<argv[0]<<": init() returned error"<<endl;

    for(int i=0; i<255; i+=6)
    {
      cout<<argv[0]<<": setting speed to "<<i<<endl;
      if( !pl.spdst( TIERproto::Speed(i, i) ) )
        cerr<<argv[0]<<": spdst() returned error"<<endl;
      else
      {
        //wait();
      };
    };

    if( !pl.ping() )
      cerr<<argv[0]<<": ping() returned error"<<endl;

    for(int i=0; i<3; ++i)
    {
      cerr<<argv[0]<<": sending stop()..."<<endl;
      pl.stop();
    };
  }
  catch(const CommProto::Exception &ex)
  {
    cerr<<argv[0]<<": CommProto exception: "<<ex.what()<<endl;
  }
  catch(...)
  {
    cerr<<argv[0]<<": unknown exception cought"<<endl;
  };

  return 0;
};


/*
 * Zuchter.cpp
 *
 */
#include <iostream>
#include <signal.h>
#include <errno.h>

#include "CommProto/ProtoLocal.hpp"
#include "TIERctl.hpp"

using namespace std;
using namespace Maps;
using namespace Path;
using namespace Zuchter;
using namespace Network;
using namespace CommProto;


// these are globals, since they are required for signal handlers
bool v4lConnOk;     // determines if connection is ok or not
bool quitProgram;   // quit program?


// generic printing listener - all other listeners will derive from it
struct PrintingListener
{
  PrintingListener(const string appName):
    _appName(appName)
  {
  };
  void print(const string &str)
  {
    cerr<<endl<<_appName<<": "<<str<<endl;
  };
private:
  const string _appName;
};


// listener for all internal classes
struct CommonPrintingListener: public  MapCreatorListener,
                               public  PathCreatorListener,
                               public  CommandCreatorListener,
                               private PrintingListener
{
  CommonPrintingListener(const string appName):
    PrintingListener(appName)
  {
  };

  // MapCreatorListener:
  virtual void onNetworkError(const std::string &why)
  {
    print(why);
    v4lConnOk=false;
  };
  virtual void onGraphicsError(const std::string &why)
  {
    print(why);
  };
  virtual void onGenericError(const std::string &why)
  {
    print(why);
  };

  // PathCreatorListener:
  virtual void onError(const std::string &why)
  {
    print(why);
  };

  // CommandCreatorListener:
  virtual void onCommError(const std::string &why)
  {
    print(why);
  };
  /*virtual void onError(const std::string &why)
  {
    print(why);
  };*/
};




static void handler_sigs(int signo)
{
  cerr<<endl<<"signal "<<signo<<" recieved: ";
  switch(signo)
  {
    case SIGPIPE:
      cerr<<"SIGPIPE";
      v4lConnOk=false;
      break;

    case SIGINT:
    case SIGTERM:
      cerr<<"SIGTERM";
      quitProgram=true;
      break;

    default:
      cerr<<"<unknown signal>";
      break;
  };
  cerr<<endl;
};


int main(const int argc, const char * const * const argv)
{
  // this signal must be ignored in order not to kill program
  // after recieveing this kind of error
  if( signal(SIGPIPE, handler_sigs)==SIG_ERR ||
      signal(SIGINT,  handler_sigs)==SIG_ERR ||
      signal(SIGTERM, handler_sigs)==SIG_ERR    )
  {
    cerr<<argv[0]<<": signal(handler_sigs): unable to change handler"<<endl;
    return 1;
  };

  try
  {

    cout<<endl;
    cout<<argv[0]<<": initializing Zuchter"<<endl;
    cout<<argv[0]<<": configuring Zuchter parameters... ";
    fflush(NULL);
    // time to sleep between reconnections trials (in [us])
    const unsigned int reconnectionsDelay=250*1000; // 0.25[s]
    // server address
    const Address addr("hell", 6669);

    fflush(NULL);
    // parameters of segmentation of image
    //CGraphics::SegmentParameters segmParams(1.3, 200, 20);
    const CGraphics::SegmentParameters segmParams(1.2, 550, 150);

    // parameters for deepth transformation of vector image
    const Graphics::Size size(130, 120);
    //Graphics::Size size(300, 250);
    const DeepthTransformParams dpthTransParams(0.21, 0.50, size, 0.08);

    // listener for map creator
    CommonPrintingListener commonListener(argv[0]);

    // map parameters
    MapCreatorParameters mapCreatorParams(commonListener, addr,
        segmParams, dpthTransParams, 10);

    // parameters for path creator:
    PathCreatorParameters pathCreatorParams(commonListener, 0.45, 0.10);

    // communication protocol to use to comunicate with robot
    ProtoLocal commProto(ProtoLocal::LPT0);

    // parameters for command creator
    const double tolerance=M_PI/18;
    CommandCreatorParameters commandCreatorParams(commonListener,
        commProto, tolerance);

    // TIER main control class
    TIERctl tCtl(mapCreatorParams,
                 pathCreatorParams,
                 commandCreatorParams);

    cout<<"done"<<endl;

    // reconnecting loop
    quitProgram=false;
    while(true)
    {
      cout<<argv[0]<<": processing: ";
      fflush(NULL);

      // main processing loop
      v4lConnOk=true;
      while(v4lConnOk && !quitProgram)
      {
        cerr<<"#";
        tCtl.process();
      };

      if(quitProgram)
        break;

      // if we're here, v4lConnOk==false
      cout<<argv[0]<<": connection broken"<<endl;
      cout<<argv[0]<<": reconnecting: ";
      fflush(NULL);
      while( !tCtl.reconnect() && !quitProgram )
      {
        cout<<"x";
        fflush(NULL);
        usleep(reconnectionsDelay);
      };
      cout<<" done"<<endl;

    }; // while(!quitProgram)
    cout<<argv[0]<<": quiting..."<<endl;
    tCtl.stopTIER();

  }
  catch(const Vectors::Exception &e)
  {
    cerr<<argv[0]<<": main(): Vectors:: "<<e.what()<<endl;
  }
  catch(const Maps::Exception &e)
  {
    cerr<<argv[0]<<": main(): Maps:: "<<e.what()<<endl;
  }
  catch(const CGraphics::Exception &e)
  {
    cerr<<argv[0]<<": main(): CGraphics:: "<<e.what()<<endl;
  }
  catch(const Graphics::Exception &e)
  {
    cerr<<argv[0]<<": main(): Graphics:: "<<e.what()<<endl;
  }
  catch(const Network::Exception &e)
  {
    cerr<<argv[0]<<": main(): Network:: "<<e.what()<<endl;
    cerr<<argv[0]<<": perror(): "<<strerror(errno)<<endl;
  }
  catch(const VCCln::Exception &e)
  {
    cerr<<argv[0]<<": main(): VCCln:: "<<e.what()<<endl;
  }
  catch(const Threads::Exception &e)
  {
    cerr<<argv[0]<<": main(): Threads:: "<<e.what()<<endl;
  }
  catch(const Path::Exception &e)
  {
    cerr<<argv[0]<<": main(): Path:: "<<e.what()<<endl;
  }
  catch(const CommProto::Exception &e)
  {
    cerr<<argv[0]<<": main(): CommProto:: "<<e.what()<<endl;
  }
  catch(...)
  {
    cerr<<argv[0]<<": main(): generic exception cought"<<endl;
  };

  return 0;
}; // main()


#include <iostream>

#include "CGraphics/Exception.hpp"
#include "CGraphics/Raster.hpp"
#include "CGraphics/Segment.hpp"

using namespace std;
using namespace CGraphics;


int main(int argc, const char * const argv[])
{
  if(argc==1)
  {
    cerr<<argv[0]<<" <img1> <img2> ..."<<endl;
    return 1;
  };

  try
  {
    for(int i=1; i<argc; ++i)
    {
      cerr<<argv[i]<<" -> ";
      Raster::AutoRaster autoIn=Raster::fromFile(argv[i]);
      Raster &rIn=*autoIn.get();
      Segment s( rIn.getSize() );
      SegmentParameters params(0.9, 500, 30);

      s.process(params, rIn);

      // write output colormap:
      Raster::AutoRaster rOut=rIn.markerToColormap();
      string outFile=string("OUTPUT_")+argv[i];
      rOut->toFile( outFile.c_str() );
      cerr<<outFile<<endl;
    };
  }
  catch(Exception &e)
  {
    cerr<<"CGraphics::Exception(): "<<e.what()<<endl;
  }
  catch(...)
  {
    cerr<<"unknown exception cought"<<endl;
  };
  return 0;
};


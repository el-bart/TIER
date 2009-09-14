/*
 * TEST.cpp
 *
 */
#include <iostream>
#include <fstream>

#include "CGraphics/Raster.hpp"
#include "CGraphics/Segment.hpp"
#include "Vectors/Line.hpp"
#include "Vectors/Point.hpp"
#include "Vectorizer.hpp"
#include "VectorImgFilter.hpp"
#include "DeepthTransformer.hpp"


using namespace std;
using namespace Maps;
using namespace CGraphics;
using namespace Vectors;


void printPoint(ostream &os, const Point2DCont &p)
{
  os<<"("<<p[0]<<";"<<p[1]<<")";
};

void printLine(ostream &os, const Line2DCont &l)
{
  os<<"[ ";
  printPoint( os, l.getFrom() );
  os<<" ; ";
  printPoint( os, l.getTo()   );
  os<<" ]";
};


void line2ps(ostream &os, const Line2DCont &l, unsigned int scale=1, int off=0)
{
  const Point2DCont from=l.getFrom();
  const Point2DCont to  =l.getTo();
  os<<scale*from[0]+off<<" "<<scale*from[1]+off<<" moveto"<<endl;
  os<<scale*to[0]  +off<<" "<<scale*to[1]  +off<<" lineto"<<endl;
};


void outputVectorImg2ps(const VectorImg &vectorImg, const char fileName[],
                        unsigned int scale=3, int off=0)
{
  ofstream os(fileName);        // open file stream

  os<<"%!PS"<<endl;
  os<<"% vector image (lines):"<<endl;
  for(unsigned int i=0; i<vectorImg.linesCnt(); ++i)
  {
    os<<endl<<"%\t"<<i<<": ";
    printLine( os, vectorImg[i] );
    os<<" @ scale=="<<scale<<endl;
    line2ps( os, vectorImg[i], scale, off );
  };
  os<<endl<<"stroke"<<endl;
};


int main(int argc, const char * const * const argv)
{
  if(argc<2)
  {
    cerr<<argv[0]<<" <file1> <file2>..."<<endl;
    return 1;
  };

  for(int fileNo=1; fileNo<argc; ++fileNo)
  {
    try
    {
      cout<<argv[0]<<": "<<argv[fileNo]<<"..."<<endl;
      // read next picture
      Raster::AutoRaster autoRaster=Raster::fromFile(argv[fileNo]);
      Raster &raster=*autoRaster.get();
      // write as BMP
      raster.toFile("OUT_raster_read.bmp");

      // segment input image
      Segment segment( raster.getSize() );
      SegmentParameters segParams(1.3, 200, 20);
      segment.process(segParams, raster);
      // output as BMP
      Raster::AutoRaster autoColormap=raster.markerToColormap();
      autoColormap->toFile("OUT_colormap.bmp");

      // prepare vectorization object
      Vectorizer vectorizer( raster.getSize() );
      // get VectorImg from raster
      VectorImg::AutoVectorImg autoVectorImg=vectorizer.vectorize(raster);
      outputVectorImg2ps( *autoVectorImg.get(), "OUTps1_vectorized.ps" );

      Graphics::Size size( raster.getSize()._x, raster.getSize()._y );
      //DeepthTransformParams dtParams(0.25, 0.40, size, 0.08);
      DeepthTransformParams dtParams(0.115, 0.26, size, 0.08);
      DeepthTransformer     dTrans(dtParams);
      autoVectorImg=dTrans.transform( *autoVectorImg.get() );
      outputVectorImg2ps( *autoVectorImg.get(), "OUTps2_deepth_map.ps",
                          200, 200 );

      Map2D map( *autoVectorImg.get() );        // make map from picture

      /*
      // transformation test for user-proveided values
      cout<<"constrains:  0<=x<="<<dTrans.getMaxX()
          <<"  0<=y<="           <<dTrans.getMaxY()<<endl;
      for(;;)
      {
        double x,y;
        cout<<"size: "<<size.x()<<" x "<<size.y()<<endl;
        cout<<"x y: "; cin>>x>>y;
        Point2DCont pIn(x,y);
        Point2DCont pOut=dTrans.transform(pIn);
        cout<<"\t("<<pIn[0] <<" ; "<<pIn[1] <<") -> ("
                   <<pOut[0]<<" ; "<<pOut[1]<<")"<<endl<<endl;
      };
      */
    }
    catch(const Vectors::Exception &e)
    {
      cerr<<argv[0]<<": Vectors:: "<<e.what()<<endl;
    }
    catch(const Maps::Exception &e)
    {
      cerr<<argv[0]<<": Maps:: "<<e.what()<<endl;
    }
    catch(const CGraphics::Exception &e)
    {
      cerr<<argv[0]<<": CGraphics:: "<<e.what()<<endl;
    }
    catch(const Graphics::Exception &e)
    {
      cerr<<argv[0]<<": Graphics:: "<<e.what()<<endl;
    }
    catch(...)
    {
      cerr<<argv[0]<<": generic exception cought"<<endl;
    };
  }; // for(all_files)

  return 0;
};


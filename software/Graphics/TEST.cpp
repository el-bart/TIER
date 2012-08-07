#include <iostream>

#include "Raster.hpp"
#include "ProcessRaster.hpp"


using namespace std;
using namespace Graphics;


void printRaster(const RasterRGB &r)
{
  const Size size=r.getSize();

  for(unsigned int y=0; y<size.y(); ++y)
  {
    for(unsigned int x=0; x<size.x(); ++x)
    {
      const Position   pos(x,y);
      PixelRGB         pixG=r.getPixelSec(pos);
      const PixelRGB  &pix =r.getPixelSec(pos);
      cout<<"["<<static_cast<int>(pix._r)
          <<" "<<static_cast<int>(pix._g)
          <<" "<<static_cast<int>(pix._b)<<"] ";
    };
    cout<<endl;
  };
  cout<<endl;
};



int main(void)
{
  try
  {

    {
      Size s1(4,5);
      Size s2(6,8);
      assert(s1!=s2);
      assert(s1<s2);
    };

    Size size(3,5);
    RasterRGB r1(size);

    for(unsigned int y=0; y<size.y(); ++y)
      for(unsigned int x=0; x<size.x(); ++x)
      {
        const Position   pos(x,y);
        PixelRGB         pixG(y, 10*y+x, x);
        r1.setPixelSec(pos, pixG);
      };

    cout<<"r1"<<endl; printRaster(r1);

    {
      RasterRGB r2(r1);
      cout<<"r1"<<endl; printRaster(r1);
      cout<<"r2"<<endl; printRaster(r2);

      Position pos(1,3);
      PixelRGB p(66, 77, 88);
      r1.setPixelSec(pos, p);

      cout<<"r1"<<endl; printRaster(r1);
      cout<<"r2"<<endl; printRaster(r2);
    };

    {
      RasterRGB::AutoRaster r2=ProcessRaster::scatterHistogram(r1);
      printRaster( *r2.get() );
    };
  }
  catch(const Graphics::Exception &e)
  {
    cerr<<"main(): Graphics::Exception cought: "<<e.what()<<endl;
  }
  catch(...)
  {
    cerr<<"main(): unknown exception cought"<<endl;
  };

  return 0;
};



/*
 * RasterDeserializer.cpp
 *
 */
#include <string.h>
#include <netinet/in.h>
#include <assert.h>

#include "RasterDeserializer.hpp"
#include "Exception.hpp"


using namespace CGraphics;
using namespace VCCln;


RasterDeserializer::RasterDeserializer(FILE *strm):
            _strm(strm)
{
  assert(strm!=NULL);
};


Raster::AutoRaster RasterDeserializer::deserialize(void)
{

  unsigned int xDim,yDim;
  char         tmp[4];

  if( fread(tmp, sizeof(tmp), 1, _strm)!=1 )
    throw Exception("RasterDeserializer::deserialize(): fread('RGB') failed");
  if( strcmp(tmp, "RGB")!=0 )
    throw Exception("RasterDeserializer::deserialize(): unknown type recieved");

  if( fread(&xDim, sizeof(xDim), 1, _strm)!=1 ||
      fread(tmp,   1,            1, _strm)!=1 ||
      fread(&yDim, sizeof(yDim), 1, _strm)!=1    )
    throw Exception("RasterDeserializer::deserialize(): read(X x Y) failed");
  // move back to host notation
  xDim=ntohl(xDim);
  yDim=ntohl(yDim);

  //
  // read raw data!
  //
  Size size(xDim, yDim);
  Raster::AutoRaster autoRaster( new Raster(size) );  // object to return
  int s_yDim=static_cast<int>(yDim);
  int s_xDim=static_cast<int>(xDim);
  for(int y=0; y<s_yDim; ++y)
    for(int x=0; x<s_xDim; ++x)
    {
      unsigned char rawRGB[3];  // R G B

      if( fread(rawRGB, sizeof(rawRGB), 1, _strm)!=1 )
        throw Exception("RasterDeserializer::deserialize(): fread(pix) failed");

      Pixel pix(rawRGB[0], rawRGB[1], rawRGB[2]);   // mk pixel
      Position pos(x,y);
      autoRaster->setPixel(pos, pix);               // write to raster
    }; // for([y;x])

  return autoRaster;
};


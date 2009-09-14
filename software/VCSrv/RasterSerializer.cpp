/*
 * RasterSerializer.cpp
 *
 */
#include <assert.h>
#include <netinet/in.h>

#include "Graphics/Size.hpp"
#include "Graphics/Pixel.hpp"
#include "Graphics/Position.hpp"
#include "RasterSerializer.hpp"
#include "Exception.hpp"


using namespace Graphics;
using namespace VCSrv;


RasterSerializer::RasterSerializer(FILE *strm):
            _strm(strm)
{
  assert(strm!=NULL);
};


void RasterSerializer::serialize(const RasterRGB &raster)
{
  const Size &size=raster.getSize();

  //
  // first write image type and size
  //
  const char imgType[]="RGB";
  if( fwrite(imgType, sizeof(imgType), 1, _strm)!=1 )
    throw Exception("RasterSerializer::serialize(): fwrite('RGB') failed");
  unsigned long tmp;
  tmp=htonl( size.x() );      // write in network notation
  if( fwrite(&tmp, sizeof(tmp), 1, _strm)!=1 )
    throw Exception("RasterSerializer::serialize(): fwrite(x) failed");
  if( fwrite("x", 1, 1, _strm)!=1 )
    throw Exception("RasterSerializer::serialize(): fwrite('x') failed");
  tmp=htonl( size.y() );      // write in network notation
  if( fwrite(&tmp, sizeof(tmp), 1, _strm)!=1 )
    throw Exception("RasterSerializer::serialize(): fwrite(y) failed");

  //
  // write raw data!
  //
  for(unsigned int y=0; y<size.y(); ++y)
    for(unsigned int x=0; x<size.x(); ++x)
    {
      const Position       pos(x,y);
      const PixelRGB      &pix   =raster.getPixel(pos);
      const unsigned char  rgb[3]={pix._r, pix._g, pix._b};

      if( fwrite(rgb, sizeof(rgb), 1, _strm)!=1 )
        throw Exception("RasterSerializer::serialize(): fwrite(y) failed");
    }; // for([y;x])

  // send it right away! we don want additional delays.
  if( fflush(_strm)!=0 )
    throw Exception("RasterSerializer::serialize(): fflush(TX) failed");
};


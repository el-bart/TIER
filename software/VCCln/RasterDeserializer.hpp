/*
 * RasterDeserializer.hpp
 *
 * class deserializing raster recieved from socket
 * (given as external stream).
 *
 */
#ifndef INCLUDE_VCCLN_RASTER_DESERIALIZER_HPP_FILE
#define INCLUDE_VCCLN_RASTER_DESERIALIZER_HPP_FILE

#include <stdio.h>

#include "CGraphics/Raster.hpp"
#include "VCCln/Exception.hpp"


namespace VCCln
{


class RasterDeserializer
{
public:
  //
  // strm - var. to read from. note: RasterDeserializer is **NOT**
  //        owner of this stream!
  //
  RasterDeserializer(FILE *strm);

  CGraphics::Raster::AutoRaster deserialize(void);

private:
  FILE *_strm;
};


}; // namespace VCCln

#endif


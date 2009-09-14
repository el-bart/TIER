/*
 * RasterSerializer.hpp
 *
 * class serializing given raster and writing it (imieadatly)
 * through socket (given as external stream.
 *
 */
#ifndef INCLUDE_VCSRV_RASTER_SERIALIZER_HPP_FILE
#define INCLUDE_VCSRV_RASTER_SERIALIZER_HPP_FILE

#include <stdio.h>

#include "Graphics/Raster.hpp"
#include "VCSrv/Exception.hpp"


namespace VCSrv
{


class RasterSerializer
{
public:
  //
  // strm - var. to write to. note: RasterSerializer is **NOT**
  //        owner of this stream!
  //
  RasterSerializer(FILE *strm);

  void serialize(const Graphics::RasterRGB &raster);

private:
  FILE *_strm;
};


}; // namespace VCSrv

#endif


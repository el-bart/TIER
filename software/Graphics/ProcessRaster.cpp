/*
 * ProcessRaster.cpp
 *
 */

#include "Graphics/ProcessRaster.hpp"
#include "Graphics/Histogram.hpp"


namespace Graphics
{


void ProcessRaster::scatterHistogram(const RasterRGB &rIn,
                                     RasterRGB &rOut)
{
  if( rIn.getSize()!=rOut.getSize() )
    throw Exception("scatterHistogram(): in/out rasters sizes does not match");

  Histogram hist(rIn);

  // do the evolution :)
};


RasterRGB::AutoRaster ProcessRaster::scatterHistogram(const RasterRGB &rIn)
{
  RasterRGB::AutoRaster autoOut( new RasterRGB( rIn.getSize() ) );
  scatterHistogram(rIn, *(autoOut.get()) );
  return autoOut;
};


}; // namespace Graphics


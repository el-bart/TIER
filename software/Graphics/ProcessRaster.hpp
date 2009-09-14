/*
 * ProcessRaster.hpp
 *
 */
#ifndef INCLUDE_GRAPHICS_PROCESSRASTER_HPP_FILE
#define INCLUDE_GRAPHICS_PROCESSRASTER_HPP_FILE

#include "Graphics/Raster.hpp"
#include "Graphics/Exception.hpp"


namespace Graphics
{


//
// class for processing immage
//
class ProcessRaster
{
public:

  // scatter histogram
  static void scatterHistogram(const Graphics::RasterRGB &rIn,
                               Graphics::RasterRGB &rOut);
  static Graphics::RasterRGB::AutoRaster
              scatterHistogram(const Graphics::RasterRGB &rIn);

private:
  ProcessRaster();  // We do not make instances

}; // class ProcessRaster



}; // namespace Graphics


#endif


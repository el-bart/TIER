/*
 * Segment.cpp
 *
 */

#include "CGraphics/Exception.hpp"
#include "Segment.hpp"
#include "segment/Helper.hpp"

#include "segment/segment-image.h"

using namespace std;
using namespace boost;
using namespace CGraphics;
using namespace SegmentWrapper;


//
// true implementation class
//
namespace CGraphics
{

class SegmentImpl
{
public:
  SegmentImpl(unsigned int width, unsigned int height);

  int process(const SegmentParameters &params,
              Raster &raster);

  inline unsigned int getDimX() { return _h._width;  };
  inline unsigned int getDimY() { return _h._height; };

private:
  Helper _h;
};


SegmentImpl::SegmentImpl(unsigned int width, unsigned int height):
        _h(width, height)
{
};


int SegmentImpl::process(const SegmentParameters &params,
                         Raster &raster)
{
  raster.scatterHistogram();
  raster.resetMarker();                     // create empty marker
                                            // must be called as the last one
                                            // before true segmentation!
  return segment_image(raster, params, _h); // process image
};



//
// pseudo-implementation (wrapper)
//

Segment::Segment(const Size &size):
        _pimpl( new SegmentImpl(size._x, size._y) )
{
};


Segment::~Segment()
{
  delete _pimpl;
};


int Segment::process(const SegmentParameters &params,
                     Raster &raster)
{
  Size rasterSize=raster.getSize();
  if( rasterSize._x!=_pimpl->getDimX() ||
      rasterSize._y!=_pimpl->getDimY()    )
    throw Exception("Segment: raster size does not match criteria");

  return _pimpl->process(params, raster);
};


}; // namespace CGraphics

